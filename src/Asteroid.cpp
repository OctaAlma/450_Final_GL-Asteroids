#include "Asteroid.h"
#include <algorithm>
#include <iostream>

#include "GLSL.h"
#include "Program.h"
#include "randomFunctions.h"

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

Asteroid::Asteroid(std::shared_ptr<Shape> &model){
    this->pos = glm::vec3(randomFloat(-MAX_X, MAX_X), 0, randomFloat(-MAX_Z, MAX_Z));
    this->dir = glm::normalize(glm::vec3((float) rand() / (RAND_MAX), 0.0f, (float) rand() / (RAND_MAX)));
    this->color = glm::vec3(randomFloat(0.0f, 1.0f), randomFloat(0.0f, 1.0f), randomFloat(0.0f, 1.0f));
    bool zNeg = rand() % 2;
    bool xNeg = rand() % 2;

    if (zNeg){
        dir[2] *= -1.0f;
    }

    if (xNeg){
        dir[0] *= -1.0f;
    }

    this->speed = randomFloat(MIN_ASTEROID_SPEED, MAX_ASTEROID_SPEED);
    this->size = randomFloat(MIN_ASTEROID_SIZE, MAX_ASTEROID_SIZE);
    this->model = model;
    this->bb = std::make_shared<BoundingBox>(this->model->getPosBuf());
}

void Asteroid::applyMVTransforms(std::shared_ptr<MatrixStack> &MV){
    MV->translate(this->pos);
    MV->scale(size, size, size);
}

void Asteroid::drawAsteroid(const std::shared_ptr<Program> prog, std::shared_ptr<MatrixStack> &MV){
    MV->pushMatrix();
    applyMVTransforms(MV);
    
    glUniformMatrix4fv(prog->getUniform("MV"), 1, GL_FALSE, glm::value_ptr(MV->topMatrix()));
	glUniform3f(prog->getUniform("kd"), color[0], color[1], color[2]);
    
    this->model->draw(prog);
    
    MV->popMatrix();
}

void Asteroid::move(){
    this->pos += this->speed * this->dir;
    
    if (this->pos.x > MAX_X){
        this->pos.x = -MAX_X;
    }
    else if (this->pos.x < -MAX_X){
        this->pos.x = MAX_X;
    }

    if (this->pos.z > MAX_Z){
        this->pos.z = -MAX_Z;
    }
    else if (this->pos.z < -MAX_Z){
        this->pos.z = MAX_Z;
    }
}

glm::vec3 Asteroid::getPos(){
    return this->pos;
}

void Asteroid::updatePos(glm::vec3 newPos){
    this->pos = newPos;
}

void Asteroid::randomPos(){
    this->pos = glm::vec3(randomFloat(-MAX_X, MAX_X), 0, randomFloat(-MAX_Z, MAX_Z));
}

void Asteroid::randomDir(){
    this->dir = glm::normalize(glm::vec3((float) rand() / (RAND_MAX), 0.0f, (float) rand() / (RAND_MAX)));
    bool zNeg = rand() % 2;
    bool xNeg = rand() % 2;

    if (zNeg){
        dir[2] *= -1.0f;
    }

    if (xNeg){
        dir[0] *= -1.0f;
    }
}