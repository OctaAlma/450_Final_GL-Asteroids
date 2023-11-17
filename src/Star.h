#pragma once
#ifndef STAR_H
#define STAR_H

#define MIN_STAR_SIZE 0.5f
#define MAX_STAR_SIZE 3.0f
#define STAR_RADIUS 150.0f
#define NUM_STARS 300

#include <vector>
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

struct Star{
	glm::vec3 pos;
	float size;

    Star();
    ~Star(){}
    void draw();
};

#endif