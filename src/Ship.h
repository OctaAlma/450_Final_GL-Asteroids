#pragma once
#ifndef SHIP_H
#define SHIP_H

#include "BoundingSphere.h"
#include "Shape.h"
#include "MatrixStack.h"
#include "ExhaustFire.h"

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>

#define UNIT 20.0f
#define INVINCIBILITY_TIME 1.0 // The number of seconds the ship is invincible after a collision

extern double tGlobal;
extern double score;
extern bool drawBoundingBox;

enum ANIMATIONS{
    NONE,
    SOMERSAULT,
    LEFT_ROLL,
    RIGHT_ROLL,
    GAME_OVER
};

class Ship: public Shape
{
    public:
        Ship(){}
        ~Ship(){}
        
        void loadMesh(const std::string &meshName);
        void initExhaust(const std::string RESOURCE_DIR);
        
        void drawShip(const std::shared_ptr<Program> prog, std::shared_ptr<MatrixStack> &MV);
        void drawFlames(std::shared_ptr<MatrixStack> &P, std::shared_ptr<MatrixStack> &MV, int width, int height, 
            std::shared_ptr<Texture> &alphaTex, std::shared_ptr<Program> &prog);
        
        void performBarrelRoll(char direction);
        void performSomersault();
        int getCurrAnim();
        
        glm::vec3 getPos();
        glm::vec3 getVel();
        glm::vec3 getForwardDir();
        float getRoll() { return this->roll; }
        float getYaw() { return this->yaw; }

        void moveShip(bool keyPresses[256]);
        void applyMVTransforms(std::shared_ptr<MatrixStack> &MV);
        MatrixStack getModelMatrix();
        void updatePrevPos();
        void setInvincible();
        bool isInvincible();

        std::shared_ptr<BoundingSphere> getBoundingSphere();

        void gameOver(std::string RESOURCE_DIR);

        void drawExplosion(std::shared_ptr<MatrixStack> &P, std::shared_ptr<MatrixStack> &MV, int width, int height, 
            std::shared_ptr<Texture> &alphaTex, std::shared_ptr<Program> &prog);

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

        double timeGameOver = INFINITY;
        std::shared_ptr<Explosion> e;
        std::vector<std::shared_ptr<ExhaustFire> > flames;
};

#endif