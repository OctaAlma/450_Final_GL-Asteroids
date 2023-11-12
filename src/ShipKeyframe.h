#pragma once
#ifndef SHIP_KEYFRAME_H
#define SHIP_KEYFRAME_H

#include "Shape.h"

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>

class ShipKeyframe
{
public:
    ShipKeyframe(){}
    ShipKeyframe(glm::vec3 p, glm::quat q);
    ~ShipKeyframe(){}

    void setPos(glm::vec3 p);
    void setQuat(glm::quat q);

    glm::vec3 getPos();
    glm::quat getQuat();
private:
    glm::vec3 p;
    glm::quat q;
};

#endif