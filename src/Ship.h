#pragma once
#ifndef SHIP_H
#define SHIP_H

#include "BoundingBox.h"
#include "Shape.h"
#include "MatrixStack.h"

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>

extern double tGlobal;

class Ship: public Shape
{
    public:
        Ship(){}
        ~Ship(){}

        void drawShip(const std::shared_ptr<Program> prog, std::shared_ptr<MatrixStack> &MV);
        void performBarrelRoll(char direction);
        void performSomersault();
        glm::vec3 getPos();
        glm::vec3 getVel();
        float getRoll() {return this->roll; }

        void moveShip(bool keyPresses[256]);
        void applyMVTransforms(std::shared_ptr<MatrixStack> &MV);
        void updatePrevPos();

    private:
        void processKeys(bool keyPresses[256]);
        glm::vec3 p_prev; // The previous position of the ship
        glm::vec3 p; // The position of the ship in space
        glm::vec3 v; // The velocity of the ship
        glm::quat rot;

        float roll = 0.0f;
        float yaw = 0.0f;
};

#endif