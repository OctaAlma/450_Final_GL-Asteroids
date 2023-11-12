#pragma once
#ifndef SHIP_H
#define SHIP_H

#include "Shape.h"
#include "MatrixStack.h"

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>

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


class Ship: public Shape
{
    public:
        Ship(){}
        ~Ship(){}

        void drawShip(const std::shared_ptr<Program> prog, std::shared_ptr<MatrixStack> &MV);
        void performBarrelRoll(char direction);
        void performSomersault();
        glm::vec3 getPos();
        glm::vec3 getVel();

        void moveShip(bool keyPresses[256]);
        void applyMVTransforms(std::shared_ptr<MatrixStack> &MV);

    private:
        void processKeys(bool keyPresses[256]);
        glm::vec3 p_prev; // The previous position of the ship
        glm::vec3 p; // The position of the ship in space
        glm::vec3 v; // The velocity of the ship
        glm::quat rot;

        float roll = 0.0f;
        float yaw = 0.0f;
};

#endif