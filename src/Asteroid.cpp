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
    this->color = glm::vec3(randomFloat(0.1f, 1.0f), randomFloat(0.1f, 1.0f), randomFloat(0.1f, 1.0f));
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
}

void Asteroid::setSize(float size){ this->size = size; }
void Asteroid::setSpeed(float speed) { this->speed = speed; }
void Asteroid::setDir(glm::vec3 dir){ this->dir = dir; }
void Asteroid::setPos(glm::vec3 pos){ this->pos = pos; }
void Asteroid::setColor(glm::vec3 color){ this->color = color; }


void Asteroid::applyMVTransforms(std::shared_ptr<MatrixStack> &MV){
    MV->translate(this->pos);
    MV->translate(0.0f, 0.0f, -5.0f * size / (MAX_ASTEROID_SIZE - MIN_ASTEROID_SIZE));
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

std::shared_ptr<BoundingSphere> Asteroid::getBoundingSphere(){
    return std::make_shared<BoundingSphere>(this->size / 0.001, this->pos);
}


std::vector<std::shared_ptr<Asteroid> > Asteroid::getChildren(){
    std::vector<std::shared_ptr<Asteroid> > children;

    if (this->size / 2.0f > MIN_ASTEROID_SIZE){
        // Create children
        auto c1 = std::make_shared<Asteroid>(this->model);
        auto c2 = std::make_shared<Asteroid>(this->model);

        // - Size = this->size / 2
        float cSize = this->size / 2.0f;
        c1->setSize(cSize);
        c2->setSize(cSize);
        
        // - Speed = this->speed * 1.2f;
        float cSpeed = std::min(this->speed * 1.2f, MAX_ASTEROID_SPEED);
        c1->setSpeed(cSpeed);
        c2->setSpeed(cSpeed);
        
        // - Direction is perpendicular to the current direction
        glm::vec3 cDir(dir.z, dir.y, dir.x);
        c1->setDir(cDir);
        c2->setDir(-1.0f * cDir);

        // - Position is equal to the parent asteroid's position
        c1->setPos(pos);
        c2->setPos(pos);

        children.push_back(c1);
        children.push_back(c2);
    }

    return children;
}