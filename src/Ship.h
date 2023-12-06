#pragma once
#ifndef SHIP_H
#define SHIP_H

#include "BoundingSphere.h"
#include "Shape.h"
#include "MatrixStack.h"

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>

#define UNIT 20.0f
#define INVINCIBILITY_TIME 1.0 // The number of seconds the ship is invincible after a collision

extern double tGlobal;
extern bool drawBoundingBox;

enum ANIMATIONS{
    NONE,
    SOMERSAULT,
    LEFT_ROLL,
    RIGHT_ROLL
};

class Ship: public Shape
{
    public:
        Ship(){}
        ~Ship(){}
        void loadMesh(const std::string &meshName);
        void drawShip(const std::shared_ptr<Program> prog, std::shared_ptr<MatrixStack> &MV);
        void performBarrelRoll(char direction);
        void performSomersault();
        glm::vec3 getPos();
        glm::vec3 getVel();
        int getCurrAnim();
        float getRoll() { return this->roll; }
        float getYaw() { return this->yaw; }
        void moveShip(bool keyPresses[256]);
        void applyMVTransforms(std::shared_ptr<MatrixStack> &MV);
        void updatePrevPos();

        void setInvincible();
        bool isInvincible();

        std::shared_ptr<BoundingSphere> getBoundingSphere();
        
    private:
        glm::vec3 p_prev; // The previous position of the ship
        glm::vec3 p; // The position of the ship in space
        glm::vec3 v; // The velocity of the ship
        float roll = 0.0f;
        float yaw = 0.0f;
        int currAnim = NONE;
        
        glm::mat4 generateEMatrix();
        void processKeys(bool keyPresses[256]);
        void setKeyframes(glm::vec3 p, int animType);
};

#endif