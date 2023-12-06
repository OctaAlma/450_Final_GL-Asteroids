#pragma once
#ifndef BOUNDING_SPHERE_H
#define BOUNDING_SPHERE_H

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

#include "Beam.h"

struct BoundingSphere{
    float radius;
    glm::vec3 center;

    BoundingSphere();
    BoundingSphere(float r, glm::vec3 c);

    bool collided(BoundingSphere &other);
    bool collided(glm::vec3 p1, glm::vec3 p2);
};

#endif