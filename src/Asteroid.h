#pragma once
#ifndef ASTEROID_H
#define ASTEROID_H

#include "BoundingBox.h"
#include "Shape.h"
#include "MatrixStack.h"

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>

// Note: MIN_X and MIN_Z are defined to be 
#define MAX_X 150.0f
#define MAX_Z 150.0f

#define MIN_ASTEROID_SPEED 0.1f
#define MAX_ASTEROID_SPEED 0.5f

#define MAX_ASTEROID_SIZE 0.01f
#define MIN_ASTEROID_SIZE 0.001f

#define NUM_ASTEROIDS 10

extern double tGlobal;
extern bool drawBoundingBox;

class Asteroid
{   
    public:
        Asteroid(std::shared_ptr<Shape> &model);
        ~Asteroid(){}
        std::shared_ptr<Shape> model;
        void drawAsteroid(const std::shared_ptr<Program> prog, std::shared_ptr<MatrixStack> &MV);
        void move();
        glm::vec3 getPos();
        void updatePos(glm::vec3 newPos);
        void applyMVTransforms(std::shared_ptr<MatrixStack> &MV);
        void randomPos();
        void randomDir();
        std::shared_ptr<BoundingBox> bb = NULL;
    private:
        glm::vec3 pos;
        glm::vec3 dir;
        glm::vec3 color;
        float size;
        float speed;
};

#endif