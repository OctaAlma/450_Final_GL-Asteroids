#include "Ship.h"
#include "ShipKeyframe.h"
#include "GLSL.h"
#include "Program.h"
#include "SplineMatrix.h"

#include <algorithm>
#include <iostream>
#include <utility> // For std::pair

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

using namespace std;
using namespace glm;

// The following variables are used to create keyframed animations for the ship:
float umax = 0.0f;
float smax = 0.0f;
double tStart = 0.0f;
double tEnd = 0.0f;

// The higher the factor, the more distance the rolls will cover
float factor = 3.0f; 

std::vector<std::shared_ptr<ShipKeyframe> > keyframes;
vector<pair<float,float> > usTable; // A table containing pairs of (u, s)

// Overrides the original loadMesh(...) function so that the Ship's bounding box can be initialized
void Ship::loadMesh(const std::string &meshName){
	Shape::loadMesh(meshName);  
	bb = make_shared<BoundingBox>(this->posBuf);
}

glm::mat4 Ship::createMVTransform(){
	auto T = make_shared<MatrixStack>();
	// Translate so that the ship intersects with the ground:
	T->translate(0.0f, -0.3f, 0.0f);
	
	// Scale the size of the ship:
	T->scale(0.7f, 0.7f, 0.7f);

	// Rotate the ship about the y axis (it will be facing the wrong direction otherwise)
	T->rotate(M_PI, 0, 1, 0);
	
	T->translate(p_prev);
	T->rotate(yaw, 0, 1, 0);
	T->translate(p - p_prev);

	if (currAnim == LEFT_ROLL){
		T->translate(generateEMatrix()[3]);
	}

	return T->topMatrix();
}

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

	// The code below is necessary for the camera to follow the ship when it rolls:
	if (currAnim == LEFT_ROLL || currAnim == RIGHT_ROLL){
		MV->translate(generateEMatrix()[3]);
	}
}

void Ship::drawShip(const std::shared_ptr<Program> prog, std::shared_ptr<MatrixStack> &MV){

	if ((tGlobal - tStart) * (2.0f + abs(v[2])) > (tEnd - tStart) && currAnim != NONE){
		
		// The code below updates the ship's position to be its position after performing the roll
		// The new position should be the ship's current position translated FACTOR * UNITS to the left or right
		glm::mat4 R = glm::rotate(glm::mat4(1.0f), yaw, glm::vec3(0, 1, 0));		
		
		if (currAnim == LEFT_ROLL){
			p_prev += glm::vec3(R * glm::vec4(factor * UNIT, 0.0f, 0.0f, 0.0f));
			p += glm::vec3(R * glm::vec4(factor * UNIT, 0.0f, 0.0f, 0.0f));
		}
		else if (currAnim == RIGHT_ROLL){
			p_prev += glm::vec3(R * glm::vec4(factor * -UNIT, 0.0f, 0.0f, 0.0f));
			p += glm::vec3(R * glm::vec4(factor * -UNIT, 0.0f, 0.0f, 0.0f));
		}

		currAnim = NONE;
	}

	MV->pushMatrix();
	applyMVTransforms(MV);

	// Below, we undo the translation to avoid the ship from moving past the camera
	if (currAnim == LEFT_ROLL || currAnim == RIGHT_ROLL){
		MV->translate(-1.0f * generateEMatrix()[3]);
	}

	if (currAnim != NONE){
		MV->multMatrix(generateEMatrix());
	}else{
		MV->rotate(-roll, 0, 0, 1);
	}

	glUniformMatrix4fv(prog->getUniform("MV"), 1, GL_FALSE, glm::value_ptr(MV->topMatrix()));
	glUniform3f(prog->getUniform("kd"), 1.0f, 1.0f, 1.0f);

	this->draw(prog);

	MV->popMatrix();

	// Updating the previous position here causes
	// p_prev = p;
}

void Ship::updatePrevPos(){
	p_prev = p;
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
	bool wPressed, aPressed, dPressed, sPressed;

	sPressed = wPressed = aPressed = dPressed = false;

	if (keyPresses[32] && currAnim == NONE){ // note: 32 is the space key
		tStart = tGlobal;
		tEnd = tGlobal + 4.0;
		performSomersault();
	}

	if ((keyPresses[(int)'q'] || keyPresses[(int)'Q']) && currAnim == NONE){ // note: 32 is the space key
		tStart = tGlobal;
		tEnd = tGlobal + 4.0;
		performBarrelRoll(LEFT_ROLL);
	}

	if ((keyPresses[(int)'e'] || keyPresses[(int)'E']) && currAnim == NONE){ // note: 32 is the space key
		tStart = tGlobal;
		tEnd = tGlobal + 4.0;
		performBarrelRoll(RIGHT_ROLL);
	}
	
	if ((currAnim == NONE) && (keyPresses[(int)'w'] || keyPresses[(int)'W'])){
		wPressed = true;
		v[2] += 0.01f;

		if (v[2] > MAX_DIR_VEL){
			v[2] = MAX_DIR_VEL;
		}
	}

	if ((currAnim == NONE) && (keyPresses[(int)'s'] || keyPresses[(int)'S'])){
		sPressed = true;
		v[2] -= 0.01f;

		if (v[2] < -(MAX_DIR_VEL / 2.0f)){
			v[2] = -(MAX_DIR_VEL / 2.0f);
		}
	}

	if ((currAnim == NONE) && (keyPresses[(int)'d'] || keyPresses[(int)'D'])){
		aPressed = true;
		this->roll -= 0.05f;

		if (roll < -MAX_ROLL){
			roll = -MAX_ROLL;
		}

		yaw += 0.05f * roll;
	}

	if ((currAnim == NONE) && (keyPresses[(int)'a'] || keyPresses[(int)'A'])){
		dPressed = true;
		this->roll += 0.05f;

		if (roll > MAX_ROLL){
			roll = MAX_ROLL;
		}

		yaw += 0.05f * roll;
	}

	// Resets the roll angle back to 0:
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

	// Resets the forward velocity to 0:
	if (!sPressed && !wPressed && (currAnim == NONE)){
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

	// Resets the left/right velocity to 0:
	if (!aPressed && !dPressed && (currAnim == NONE)){
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

void createKeyframes(){
	// We are only going to need 6 keyframes for the somersault and barrel rolls:
	for (int i = 0; i < 8; i++){
		keyframes.push_back(std::make_shared<ShipKeyframe>());
	}

	umax = keyframes.size() - 3;
}

void buildTable()
{
	usTable.clear();

	// Set B to be the appropriate matrix:
	glm::mat4 B = createCatmull();
	usTable.push_back(make_pair(0.0f, 0.0f));

	// Compute using approximations:
	for (int i = 0; i < keyframes.size() - 3; i++){	

		glm::mat4 G(0);
		G[0] = glm::vec4(keyframes.at(0 + i)->getPos(), 0);
		G[1] = glm::vec4(keyframes.at(1 + i)->getPos(), 0);
		G[2] = glm::vec4(keyframes.at(2 + i)->getPos(), 0);
		G[3] = glm::vec4(keyframes.at(3 + i)->getPos(), 0);

		float ua = 0.0f;
		for (float ub = 0.2f; ub <= 1.0f; ub = ub + 0.2f){
			glm::vec4 uaVec(1, ua, ua * ua, ua * ua * ua);
			glm::vec4 ubVec(1, ub, ub * ub, ub * ub * ub);
			glm::vec3 P_ua = G * B * uaVec;
			glm::vec3 P_ub = G * B * ubVec;
			float s = glm::length(P_ua - P_ub);
			usTable.push_back(make_pair(ub + i, s + usTable.at(usTable.size()-1).second));
			ua += 0.2f;
		}
	}

	smax = usTable.at(usTable.size()-1).second;
}

// Set the position and rotations of each keyframe in the keyframes vector
// NOTE: THE QUATERNION ROTATIONS HAVE NOT YET BEEN SET
void Ship::setKeyframes(glm::vec3 p, int animType){
	glm::vec3 xAxis = glm::vec3(1, 0, 0);

	switch(animType){
		case (NONE):{
			break;
		}

		case(SOMERSAULT):{
			currAnim = animType;
			// Control point behind the current position. Same orientation
			keyframes.at(0)->setPos(glm::vec3(0, 0, 0));
			keyframes.at(1)->setQuat(glm::angleAxis(0.0f, xAxis));
			
			// Control point that is the current position. Same orientation
			keyframes.at(1)->setPos(glm::vec3(0, 0, 0));
			keyframes.at(1)->setQuat(glm::angleAxis(0.0f, xAxis));

			// Control point that is one unit on top and one unit in front. (Ship facing upward)
			keyframes.at(2)->setPos(glm::vec3(0.0f, UNIT / 2.0f, UNIT / 2.0f));
			keyframes.at(2)->setQuat(glm::angleAxis(-(float)M_PI_2, xAxis));

			// Control point at the top (Upside down orientation)
			keyframes.at(3)->setPos(glm::vec3(0.0f, UNIT, 0.0f));
			keyframes.at(3)->setQuat(glm::angleAxis(-(float)M_PI, xAxis));

			// Control point that is one unit on top and one unit behind. (Ship facing downward)
			keyframes.at(4)->setPos(glm::vec3(0.0f, UNIT / 2.0f, -UNIT / 2.0f));
			keyframes.at(4)->setQuat(glm::angleAxis(-(float)M_PI -(float)M_PI_2, xAxis));

			// Control point that is the current position. Same orientation
			keyframes.at(5)->setPos(glm::vec3(0, 0, 0));
			keyframes.at(5)->setQuat(glm::angleAxis(0.0f, xAxis));
			
			// Control point in front of the current position. Same orientation
			keyframes.at(6)->setPos(glm::vec3(0, 0, 0));
			keyframes.at(6)->setQuat(glm::angleAxis(0.0f, xAxis));
			break;
		}
	
		case(LEFT_ROLL):{
			glm::vec3 rotAxis = glm::vec3(0, 0, 1);

			currAnim = animType;
			// Control frame at the original position of the ship
			keyframes.at(0)->setPos(glm::vec3(0, 0, 0));
			keyframes.at(0)->setQuat(glm::angleAxis(0.0f, rotAxis));
			// Another control frame at the original position of the ship
			keyframes.at(1)->setPos(glm::vec3(0, 0, 0));
			keyframes.at(1)->setQuat(glm::angleAxis(0.0f, rotAxis));

			// Control frame at 1/3 of the way there. Rotated about the Z axis by PI/4
			keyframes.at(2)->setPos(glm::vec3(factor * UNIT/4.0f, 0, 0));
			keyframes.at(2)->setQuat(glm::angleAxis(5.0f * (float)M_PI_4, rotAxis));

			// Control frame at 1/2 of the way there. Rotates about the Z axis by PI
			keyframes.at(3)->setPos(glm::vec3(factor * UNIT/2.0f, 0, 0));
			keyframes.at(3)->setQuat(glm::angleAxis((float)M_PI, rotAxis));

			// Control frame at 2/3 of the way there. Rotated on the Z axis by 5PI/4
			keyframes.at(4)->setPos(glm::vec3(factor * 3.0f * UNIT/4.0f, 0, 0));
			keyframes.at(4)->setQuat(glm::angleAxis((float)M_PI_4, rotAxis));

			// Control frame at the position of the ship translated by UNITS in the x axis
			keyframes.at(5)->setPos(glm::vec3(factor * UNIT, 0, 0));
			keyframes.at(5)->setQuat(glm::angleAxis(0.0f, rotAxis));
			// Another control frame at the position of the ship translated by UNITS in the x axis
			keyframes.at(6)->setPos(glm::vec3(factor * UNIT, 0, 0));
			keyframes.at(6)->setQuat(glm::angleAxis(0.0f, rotAxis));
			break;
		}

		case(RIGHT_ROLL):{
			glm::vec3 rotAxis = glm::vec3(0, 0, 1);

			currAnim = animType;
			// Control frame at the original position of the ship
			keyframes.at(0)->setPos(glm::vec3(0, 0, 0));
			keyframes.at(0)->setQuat(glm::angleAxis(0.0f, rotAxis));
			// Another control frame at the original position of the ship
			keyframes.at(1)->setPos(glm::vec3(0, 0, 0));
			keyframes.at(1)->setQuat(glm::angleAxis(0.0f, rotAxis));

			// Control frame at 1/3 of the way there. Rotated about the Z axis by PI/4
			keyframes.at(2)->setPos(glm::vec3(factor * -UNIT/4.0f, 0, 0));
			keyframes.at(2)->setQuat(glm::angleAxis((float)M_PI_4, rotAxis));

			// Control frame at 1/2 of the way there. Rotates about the Z axis by PI
			keyframes.at(3)->setPos(glm::vec3(factor * -UNIT/2.0f, 0, 0));
			keyframes.at(3)->setQuat(glm::angleAxis((float)M_PI, rotAxis));

			// Control frame at 2/3 of the way there. Rotated on the Z axis by 5PI/4
			keyframes.at(4)->setPos(glm::vec3(factor * -3.0f * UNIT/4.0f, 0, 0));
			keyframes.at(4)->setQuat(glm::angleAxis(5.0f * (float)M_PI_4, rotAxis));

			// Control frame at the position of the ship translated by UNITS in the x axis
			keyframes.at(5)->setPos(glm::vec3(factor * -UNIT, 0, 0));
			keyframes.at(5)->setQuat(glm::angleAxis(0.0f, rotAxis));
			// Another control frame at the position of the ship translated by UNITS in the x axis
			keyframes.at(6)->setPos(glm::vec3(factor * -UNIT, 0, 0));
			keyframes.at(6)->setQuat(glm::angleAxis(0.0f, rotAxis));
			break;
		}
	}

	if (animType != NONE){
		// Make sure adjacent quaternions created will not do the 'twirl' between each other
		for (int i = 0; i < keyframes.size() - 1; i++){
			if (dot(keyframes.at(i)->getQuat(), keyframes.at(i + 1)->getQuat()) <= 0.0f){
				keyframes.at(i + 1)->setQuat(-1.0f * keyframes.at(i + 1)->getQuat());
			}
		}

		buildTable();
	}
}

glm::mat4 Ship::generateEMatrix(){
	float u = std::fmod((tGlobal - tStart) * (2.0f + abs(v[2])), umax);
	int k = floor(u);
	float u_hat = u - k; // u_hat is between 0 and 1
	mat4 B = createCatmull();
	vec4 uVec(1.0f, u_hat, u_hat * u_hat, u_hat * u_hat * u_hat);
	mat4 G(0);

	// Define G for rotations
	G[0] = glm::vec4(keyframes.at((0 + k))->getQuat().x, keyframes.at((0 + k))->getQuat().y, keyframes.at((0 + k))->getQuat().z, keyframes.at((0 + k))->getQuat().w);
	G[1] = glm::vec4(keyframes.at((1 + k))->getQuat().x, keyframes.at((1 + k))->getQuat().y, keyframes.at((1 + k))->getQuat().z, keyframes.at((1 + k))->getQuat().w);
	G[2] = glm::vec4(keyframes.at((2 + k))->getQuat().x, keyframes.at((2 + k))->getQuat().y, keyframes.at((2 + k))->getQuat().z, keyframes.at((2 + k))->getQuat().w);
	G[3] = glm::vec4(keyframes.at((3 + k))->getQuat().x, keyframes.at((3 + k))->getQuat().y, keyframes.at((3 + k))->getQuat().z, keyframes.at((3 + k))->getQuat().w);

	glm::vec4 qVec = G * (B * uVec);
	glm::quat q(qVec[3], qVec[0], qVec[1], qVec[2]); // Constructor argument order: (w, x, y, z)
	glm::mat4 E = glm::mat4_cast(glm::normalize(q)); // Creates a rotation matrix

	// Define G for translations
	G[0] = glm::vec4(keyframes.at((0 + k))->getPos(), 0);
	G[1] = glm::vec4(keyframes.at((1 + k))->getPos(), 0);
	G[2] = glm::vec4(keyframes.at((2 + k))->getPos(), 0);
	G[3] = glm::vec4(keyframes.at((3 + k))->getPos(), 0);

	glm::vec3 pos = G * B * uVec;

	E[3] = glm::vec4(pos, 1.0f); // Puts the position into the last column

	return E;
}

void Ship::performBarrelRoll(char direction)
{
	if (keyframes.empty()){ createKeyframes(); }

	if (direction == LEFT_ROLL){
		setKeyframes(this->p, LEFT_ROLL);
	}
	else if (direction == RIGHT_ROLL){
		setKeyframes(this->p, RIGHT_ROLL);
	}
}

void Ship::performSomersault()
{
	if (keyframes.empty()){ createKeyframes(); }

	if (currAnim == NONE){ setKeyframes(this->p, SOMERSAULT); }
}
