#include "BoundingSphere.h"
#include <iostream>

using std::cout, std::endl;

BoundingSphere::BoundingSphere(): radius(0.0f), center(0.0f, 0.0f, 0.0f) {}

BoundingSphere::BoundingSphere(float r, glm::vec3 c): radius(r), center(c) {}

bool BoundingSphere::collided(BoundingSphere &other){

    const float dist = sqrt(
        (this->center.x - other.center.x) * (this->center.x - other.center.x) +
        (this->center.y - other.center.y) * (this->center.y - other.center.y) +
        (this->center.z - other.center.z) * (this->center.z - other.center.z)
    );

    return dist <= (this->radius + other.radius);
}

void printVec(glm::vec3 v){
    cout << v.x << ", " << v.y << ", " << v.z << endl;
}

// Checks if a line segment characterized by two points intersects the sphere
// Implementation obtained here: https://www.baeldung.com/cs/circle-line-segment-collision-detection
// Alternate formula for minDist obtained here: https://mathworld.wolfram.com/Point-LineDistance3-Dimensional.html
bool BoundingSphere::collided(glm::vec3 p1, glm::vec3 p2){

    // Finds the shortest distance from a line specified by p1 and p2 to the center of the sphere
    // If this distance is less than or equal to the sphere's radius, we have an intersection    

    float dist_p1_c = glm::distance(center, p1);
    float dist_p2_c = glm::distance(center, p2);

    float minDist = INFINITY;
    float maxDist = std::max(dist_p1_c, dist_p2_c);

    if (glm::dot(p1 - center, p1 - p2) > 0.0f && glm::dot(p2 - center, p2 - p1) > 0.0f){
        minDist = glm::length(glm::cross(center - p1, center - p2)) / glm::length(p2 - p1);

    }else{
        minDist = std::min(dist_p1_c, dist_p2_c);
    }

    return minDist <= radius && maxDist >= radius;
}