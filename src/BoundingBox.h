#pragma once
#ifndef BOUNDING_BOX_H
#define BOUNDING_BOX_H

#include "Shape.h"
#include <memory>

#include <vector>
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

struct BoundingBox{
	glm::vec3 minCoords;
	glm::vec3 maxCoords;

	BoundingBox(std::vector<float> &posBuf);
    BoundingBox(std::vector<float>* posBuf);

    void draw();
};

#endif