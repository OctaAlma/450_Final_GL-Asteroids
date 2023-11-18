#include "Beam.h"

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

Beam::Beam(){
    origin = glm::vec3(0.0f, 0.0f, 0.0f);
    dir = glm::vec3(0.0f, 0.0f, 0.0f);

    speed = BEAM_SPEED;
    thickness = BEAM_THICKNESS;
    length = BEAM_LENGTH;
    tCreated = INFINITY;
}

Beam::Beam(glm::vec3 origin, glm::vec3 dir, bool dead){
    this->origin = origin;
    this->dir = glm::normalize(dir);

    speed = BEAM_SPEED;
    thickness = BEAM_THICKNESS;
    length = BEAM_LENGTH;

    if (dead){ tCreated = INFINITY; }
    else{ tCreated = tGlobal; }
}

void Beam::setThickness(float t){ this->thickness = t; }

bool Beam::isAlive(){ return ((tCreated != INFINITY) && ((tGlobal - tCreated) < BEAM_LIFE)); } 

void Beam::draw(){
    if (!isAlive()){ return; }

    glm::vec3 start = origin + speed * (float)(tGlobal - tCreated) * dir;
    glm::vec3 end = origin + length * glm::normalize(dir) + speed * (float)(tGlobal - tCreated) * dir;

    glLineWidth(thickness);
    glColor3f(1.0, 0.0f, 0.0f);

    glBegin(GL_LINES);

    glVertex3f(start.x, start.y, start.z);
    glVertex3f(end.x, end.y, end.z);

    glEnd();
}

void Beam::reset(glm::vec3 origin, glm::vec3 dir){
    this->origin = origin;
    this->dir = glm::normalize(dir);
    tCreated = tGlobal;
}