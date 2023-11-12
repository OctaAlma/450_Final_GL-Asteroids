#include "Ship.h"
#include <algorithm>
#include <iostream>

#include "GLSL.h"
#include "Program.h"

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

using namespace std;

void Ship::applyMVTransforms(std::shared_ptr<MatrixStack> &MV){
	// Translate so that the ship intersects with the ground:
	MV->translate(0.0f, -0.3f, 0.0f);
	
	// Scale the size of the ship:
	MV->scale(0.7f, 0.7f, 0.7f);

	// Rotate the ship about the y axis (it will be facing the wrong direction otherwise)
	MV->rotate(M_PI, 0, 1, 0);
	
	MV->translate(p_prev);
	MV->rotate(yaw, 0, 1, 0);
	MV->translate(p - p_prev);
}

void Ship::drawShip(const std::shared_ptr<Program> prog, std::shared_ptr<MatrixStack> &MV){
	MV->pushMatrix();

	applyMVTransforms(MV);
	MV->rotate(-roll, 0, 0, 1);

	glUniformMatrix4fv(prog->getUniform("MV"), 1, GL_FALSE, glm::value_ptr(MV->topMatrix()));
	glUniform3f(prog->getUniform("kd"), 1.0f, 1.0f, 1.0f);

	this->draw(prog);

	MV->popMatrix();

	p_prev = p;
}

void Ship::performBarrelRoll(char direction)
{

}

void Ship::performSomersault()
{

}

glm::vec3 Ship::getPos(){ return this->p; }

glm::vec3 Ship::getVel(){ return this->v; }

#define MAX_DIR_VEL 1.0f
#define MAX_ROLL M_PI_4

void Ship::moveShip(bool keyPresses[256]){
	processKeys(keyPresses);
	glm::mat4 R = glm::rotate(glm::mat4(1.0f), yaw, glm::vec3(0, 1, 0));
	p = p + glm::vec3(R * glm::vec4(v, 0.0f));
}

void Ship::processKeys(bool keyPresses[256]){
	bool spacePressed, wPressed, aPressed, dPressed, sPressed;

	sPressed = spacePressed = wPressed = aPressed = dPressed = false;

	// note: 32 is the space key
	
	if (keyPresses[(int)'w'] || keyPresses[(int)'W']){
		wPressed = true;
		v[2] += 0.01f;

		if (v[2] > MAX_DIR_VEL){
			v[2] = MAX_DIR_VEL;
		}
	}

	if (keyPresses[(int)'s'] || keyPresses[(int)'S']){
		sPressed = true;
		v[2] -= 0.01f;

		if (v[2] < -(MAX_DIR_VEL / 2.0f)){
			v[2] = -(MAX_DIR_VEL / 2.0f);
		}
	}

	if (keyPresses[(int)'d'] || keyPresses[(int)'D']){
		aPressed = true;
		this->roll -= 0.1f;

		if (roll < -MAX_ROLL){
			roll = -MAX_ROLL;
		}

		yaw += 0.05f * roll;
	}

	if (keyPresses[(int)'a'] || keyPresses[(int)'A']){
		dPressed = true;
		this->roll += 0.1f;

		if (roll > MAX_ROLL){
			roll = MAX_ROLL;
		}

		yaw += 0.05f * roll;
	}

	if (!aPressed && !dPressed){
		if (roll != 0.0f){
			if (roll < 0.0f){
				roll += 0.05f;
			}else{
				roll -= 0.05f;
			}

			if (abs(roll - 0.02f) <= 0.1f){
				roll = 0.0f;
			}
		}
	}

	if (!sPressed && !wPressed){
		if (v[2] != 0.0f){
			if (v[2] < 0.0f){
				v[2] += 0.007f;
			}else{
				v[2] -= 0.007f;
			}

			if (abs(v[2] - 0.01f) <= 0.02f){
				v[2] = 0.0f;
			}
		}
	}

	if (!aPressed && !dPressed){
		if (v[1] != 0.0f){
			if (v[1] < 0.0f){
				v[1] += 0.007f;
			}else{
				v[1] -= 0.007f;
			}

			if (abs(v[1] - 0.01f) <= 0.02f){
				v[1] = 0.0f;
			}
		}
	}

}