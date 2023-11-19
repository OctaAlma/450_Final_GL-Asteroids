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

    float yRot = 0.0f;

	BoundingBox(std::vector<float> &posBuf);
    BoundingBox(std::vector<float>* posBuf);
    BoundingBox(glm::vec3 p1, glm::vec3 p2);

    void updateCoords(std::shared_ptr<MatrixStack> M);
    void updateCoords(glm::mat4 M);

    void draw();
    // Given another bounding box, checks if they collide
    // Assumes they are in the same coordinate system
    bool collided(std::shared_ptr<BoundingBox> other);
};

#endif