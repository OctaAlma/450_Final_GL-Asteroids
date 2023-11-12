#include "Asteroid.h"
#include <algorithm>
#include <iostream>

#include "GLSL.h"
#include "Program.h"

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

// The following was obtained from:
// https://stackoverflow.com/questions/5289613/generate-random-float-between-two-floats
float randomFloat(float a, float b) {
    float random = ((float) rand()) / (float) RAND_MAX;
    float diff = b - a;
    float r = random * diff;
    return a + r;
}

Asteroid::Asteroid(){
    this->pos = glm::vec3(randomFloat(-MAX_X, MAX_X), 0, randomFloat(-MAX_Z, MAX_Z));
    this->dir = glm::normalize(glm::vec3((float) rand() / (RAND_MAX), 0.0f, (float) rand() / (RAND_MAX)));

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
}

void Asteroid::drawAsteroid(const std::shared_ptr<Program> prog, std::shared_ptr<MatrixStack> &MV){
    MV->pushMatrix();
    MV->translate(this->pos);
    MV->scale(size, size, size);
    
    glUniformMatrix4fv(prog->getUniform("MV"), 1, GL_FALSE, glm::value_ptr(MV->topMatrix()));
	glUniform3f(prog->getUniform("kd"), 1.0f, 1.0f, 1.0f);

    this->draw(prog);
    
    MV->popMatrix();
}

void Asteroid::move(){
    this->pos += this->speed * this->dir;
}

glm::vec3 Asteroid::getPos(){
    return this->pos;
}