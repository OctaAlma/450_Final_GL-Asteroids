#include "ShipKeyframe.h"

ShipKeyframe::ShipKeyframe(glm::vec3 p, glm::quat q){
    this->p = p;
    this->q = q;
}

void ShipKeyframe::setPos(glm::vec3 p){
    this->p = p;
}

void ShipKeyframe::setQuat(glm::quat q){
    this->q = q;
}

glm::vec3 ShipKeyframe::getPos(){
    return this->p;
}

glm::quat ShipKeyframe::getQuat(){
    return this->q;
}