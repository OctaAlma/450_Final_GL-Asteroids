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

#define MAX_X 500.0f
#define MAX_Z 500.0f

#define MIN_ASTEROID_SPEED 0.1f
#define MAX_ASTEROID_SPEED 0.5f

#define MAX_ASTEROID_SIZE 0.01f
#define MIN_ASTEROID_SIZE 0.001f

#define NUM_ASTEROIDS 20

class Asteroid: public Shape
{   
    public:
        Asteroid();
        ~Asteroid(){}
        void drawAsteroid(const std::shared_ptr<Program> prog, std::shared_ptr<MatrixStack> &MV);
        void move();
        glm::vec3 getPos();
    private:
        glm::vec3 pos;
        glm::vec3 dir;
        float size;
        float speed;
};

#endif