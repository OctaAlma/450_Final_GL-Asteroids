#include "Beam.h"

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include "MatrixStack.h"

using namespace glm;
using namespace std;

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

bool Beam::isAlive(){ return ((tCreated != INFINITY) && (tCreated + BEAM_LIFE > tGlobal)); } 

void Beam::draw(){
    if (!isAlive()){ return; }

    vec3 disp = speed * (float)(tGlobal - tCreated) * dir;
    vec3 start = origin + disp;
    vec3 end = origin + disp + length * normalize(dir);

    glLineWidth(thickness);
    glColor3f(1.0f, 0.0f, 0.0f);

    // The following causes the beam's color to fade 
    float p = (tGlobal - tCreated) / (BEAM_LIFE);
    glColor3f(1.0f - p, 0.0f, 0.0f);

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

glm::vec3 Beam::getStart(){
    vec3 disp = speed * (float)(tGlobal - tCreated) * dir;
    vec3 start = origin + disp;
    return start;
}

glm::vec3 Beam::getEnd(){
    vec3 disp = speed * (float)(tGlobal - tCreated) * dir;
    vec3 end = origin + disp + length * normalize(dir);
    return end;
}