#pragma once
#ifndef ASTEROID_H
#define ASTEROID_H

#include "BoundingSphere.h"
#include "Shape.h"
#include "MatrixStack.h"

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>

// Note: MIN_X and MIN_Z are defined to be 
#define MAX_X 110.0f
#define MAX_Z 110.0f

#define MIN_ASTEROID_SPEED 0.2f
#define MAX_ASTEROID_SPEED 0.4f

#define MAX_ASTEROID_SIZE 0.015f
#define MIN_ASTEROID_SIZE 0.005f

extern int NUM_ASTEROIDS;

extern double tGlobal;
extern bool drawBoundingBox;

class Asteroid
{   
    public:
        Asteroid(std::shared_ptr<Shape> &model);
        ~Asteroid(){}

        void setSize(float size);
        void setSpeed(float speed);
        void setDir(glm::vec3 dir);
        void setPos(glm::vec3 pos);
        void setColor(glm::vec3 color);
        glm::vec3 getColor() { return this->color; }

        std::shared_ptr<Shape> model;
        void drawAsteroid(const std::shared_ptr<Program> prog, std::shared_ptr<MatrixStack> &MV);
        void move();
        glm::vec3 getPos();
        void updatePos(glm::vec3 newPos);
        void applyMVTransforms(std::shared_ptr<MatrixStack> &MV);
        void randomPos();
        void randomDir();
        std::shared_ptr<BoundingSphere> getBoundingSphere();
        std::vector<std::shared_ptr<Asteroid>> getChildren();
    private:
        glm::vec3 pos;
        glm::vec3 dir;
        glm::vec3 color;
        float size;
        float speed;
};

#endif