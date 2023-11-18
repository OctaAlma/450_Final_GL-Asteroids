#pragma once
#ifndef BEAM_H
#define BEAM_H

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

#define MAX_BEAMS 15
#define BEAM_LIFE 2.0 // Maximum time a beam will live (in seconds)
#define BEAM_SPEED 150.0f
#define BEAM_THICKNESS 3.0f
#define BEAM_LENGTH 3.0f

extern double tGlobal;

class Beam
{
public:
    Beam();
    Beam(glm::vec3 origin, glm::vec3 dir, bool dead = false);

    void setThickness(float t);
    void draw();
    bool isAlive();
    void reset(glm::vec3 origin, glm::vec3 dir);

private:
    glm::vec3 origin;
    glm::vec3 dir;

    float speed;
    float thickness;
    float length;
    double tCreated = -1.0;
};

#endif