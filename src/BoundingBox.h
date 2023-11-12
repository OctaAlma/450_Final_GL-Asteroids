#pragma once
#ifndef BOUNDING_BOX_H
#define BOUNDING_BOX_H

#include <vector>
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

struct BoundingBox{
	glm::vec3 minCoords;
	glm::vec3 maxCoords;

	BoundingBox(std::vector<float> posBuf){
		assert(posBuf.size() % 3 == 0);

        this->minCoords = glm::vec3(posBuf[0], posBuf[1], posBuf[2]);
        this->maxCoords = glm::vec3(posBuf[0], posBuf[1], posBuf[2]);
        for (int i = 3; i < posBuf.size(); i += 3){
            glm::vec3 currVert(posBuf[i], posBuf[i+1], posBuf[i+2]);

            if (currVert.x < minCoords.x){ minCoords.x = currVert.x; }
            if (currVert.y < minCoords.y){ minCoords.y = currVert.y; }
            if (currVert.z < minCoords.z){ minCoords.z = currVert.z; }

            if (currVert.x > maxCoords.x){ maxCoords.x = currVert.x; }
            if (currVert.y > maxCoords.y){ maxCoords.y = currVert.y; }
            if (currVert.z > maxCoords.z){ maxCoords.z = currVert.z; }
        }		
	}
};

#endif