#pragma once
#ifndef BOUNDING_BOX_H
#define BOUNDING_BOX_H

#include "Shape.h"
#include "MatrixStack.h"
#include <memory>

#include <vector>
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

struct BoundingBox{
	glm::vec3 minCoords;
	glm::vec3 maxCoords;

	BoundingBox(std::vector<float> &posBuf);
    BoundingBox(std::vector<float>* posBuf);

    void updateCoords(std::shared_ptr<MatrixStack> M);

    void draw();
    bool collided(std::shared_ptr<BoundingBox> other);
};

#endif