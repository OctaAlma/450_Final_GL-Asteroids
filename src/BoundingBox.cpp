#include "BoundingBox.h"

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

using namespace std;

BoundingBox::BoundingBox(std::vector<float> &posBuf){
    assert(posBuf.size() % 3 == 0);

    this->minCoords = glm::vec3(posBuf[0], posBuf[1], posBuf[2]);
    this->maxCoords = glm::vec3(posBuf[0], posBuf[1], posBuf[2]);
    for (int i = 3; i < posBuf.size(); i += 3){
        glm::vec3 currVert(posBuf[i], posBuf[i+1], posBuf[i+2]);

        if (currVert.x < minCoords.x){ minCoords.x = currVert.x; }
        if (currVert.y < minCoords.y){ minCoords.y = currVert.y; }
        if (currVert.z < minCoords.z){ minCoords.z = currVert.z; }

        if (currVert.x > maxCoords.x){ maxCoords.x = currVert.x; }
        if (currVert.y > maxCoords.y){ maxCoords.y = currVert.y; }
        if (currVert.z > maxCoords.z){ maxCoords.z = currVert.z; }
    }		
}

BoundingBox::BoundingBox(std::vector<float>* posBuf){
    assert(posBuf->size() % 3 == 0);

    this->minCoords = glm::vec3(posBuf->at(0), posBuf->at(1), posBuf->at(0));
    this->maxCoords = glm::vec3(posBuf->at(0), posBuf->at(1), posBuf->at(2));
    for (int i = 3; i < posBuf->size(); i += 3){
        glm::vec3 currVert(posBuf->at(i), posBuf->at(i + 1), posBuf->at(i + 2));

        if (currVert.x < minCoords.x){ minCoords.x = currVert.x; }
        if (currVert.y < minCoords.y){ minCoords.y = currVert.y; }
        if (currVert.z < minCoords.z){ minCoords.z = currVert.z; }

        if (currVert.x > maxCoords.x){ maxCoords.x = currVert.x; }
        if (currVert.y > maxCoords.y){ maxCoords.y = currVert.y; }
        if (currVert.z > maxCoords.z){ maxCoords.z = currVert.z; }
    }	
}

void BoundingBox::updateCoords(std::shared_ptr<MatrixStack> M){
    minCoords = M->topMatrix() * glm::vec4(minCoords, 1.0f);
    maxCoords = M->topMatrix() * glm::vec4(maxCoords, 1.0f);
}

void BoundingBox::draw(){
    // Draw frame
	glLineWidth(2);
    glColor3f(1, 1, 1);
	glBegin(GL_LINES);

    float xCoords[2] = {minCoords.x, maxCoords.x};
    float yCoords[2] = {minCoords.y, maxCoords.y};
    float zCoords[2] = {minCoords.z, maxCoords.z};

    float xCurr, yCurr, zCurr;

    // Draw E1:    
	glVertex3f(minCoords.x, minCoords.y, minCoords.z);
	glVertex3f(maxCoords.x, minCoords.y, minCoords.z);
	
    // Draw E2
    glVertex3f(minCoords.x, minCoords.y, minCoords.z);
	glVertex3f(minCoords.x, maxCoords.y, minCoords.z);

    // Draw E3
    glVertex3f(minCoords.x, minCoords.y, minCoords.z);
	glVertex3f(minCoords.x, minCoords.y, maxCoords.z);

    // Draw E4
    glVertex3f(maxCoords.x, minCoords.y, minCoords.z);
	glVertex3f(maxCoords.x, maxCoords.y, minCoords.z);

    // Draw E5
    glVertex3f(maxCoords.x, minCoords.y, minCoords.z);
	glVertex3f(maxCoords.x, minCoords.y, maxCoords.z);

    // Draw E6
    glVertex3f(minCoords.x, maxCoords.y, minCoords.z);
	glVertex3f(maxCoords.x, maxCoords.y, minCoords.z);

    // Draw E7
    glVertex3f(minCoords.x, maxCoords.y, minCoords.z);
	glVertex3f(minCoords.x, maxCoords.y, maxCoords.z);

    // Draw E8
    glVertex3f(minCoords.x, minCoords.y, maxCoords.z);
	glVertex3f(maxCoords.x, minCoords.y, maxCoords.z);

    // Draw E9
    glVertex3f(minCoords.x, minCoords.y, maxCoords.z);
	glVertex3f(minCoords.x, maxCoords.y, maxCoords.z);

    // Draw E10
    glVertex3f(minCoords.x, maxCoords.y, maxCoords.z);
	glVertex3f(maxCoords.x, maxCoords.y, maxCoords.z);

    // Draw E11
    glVertex3f(maxCoords.x, maxCoords.y, maxCoords.z);
	glVertex3f(maxCoords.x, minCoords.y, maxCoords.z);
    
    // Draw E12
    glVertex3f(maxCoords.x, maxCoords.y, minCoords.z);
	glVertex3f(maxCoords.x, maxCoords.y, maxCoords.z);

    glEnd();
}

bool BoundingBox::collided(std::shared_ptr<BoundingBox> other){ 

    // xmax1 >= xmin2 and xmax2 >= xmin1
    bool cond1 = (maxCoords.x >= other->minCoords.x && other->maxCoords.x >= minCoords.x);
    // ymax1 >= ymin2 and ymax2 >= ymin1
    bool cond2 = (maxCoords.y >= other->minCoords.y && other->maxCoords.y >= minCoords.y);
    // zmax1 >= zmin2 and zmax2 >= zmin1
    bool cond3 = (maxCoords.z >= other->minCoords.z && other->maxCoords.z >= minCoords.z);

    return cond1 && cond2 && cond3; 
}