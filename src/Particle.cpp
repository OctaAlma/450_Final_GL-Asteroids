#include "Particle.h"

#include <iostream>

#include "GLSL.h"
#include "MatrixStack.h"
#include "Program.h"
#include "Texture.h"

using namespace std;
using namespace Eigen;

// Before this constructor is called, posBuf must be a valid vector<float>.
// I.e., Particle::init(n) must be called first.
Particle::Particle(int index, GLuint colBufID, GLuint scaBufID, std::vector<float> &posBuf, std::vector<float> &colBuf, 
	std::vector<float> &alpBuf, std::vector<float> &scaBuf, Eigen::Vector3f col):
		color(&colBuf[3*index]),
		scale(scaBuf[index]),
		x(&posBuf[3*index]),
		alpha(alpBuf[index])
{
	// Random fixed properties
	color = col;
	scale = randFloat(MIN_PARTICLE_SIZE, MAX_PARTICLE_SIZE);
	lifespan = randFloat(MIN_PARTICLE_LIFESPAN, MAX_PARTICLE_LIFESPAN);
	
	// Send color data to GPU
	glBindBuffer(GL_ARRAY_BUFFER, colBufID);
	glBufferSubData(GL_ARRAY_BUFFER, 3*index*sizeof(float), 3*sizeof(float), color.data());
	
	// Send scale data to GPU
	glBindBuffer(GL_ARRAY_BUFFER, scaBufID);
	glBufferSubData(GL_ARRAY_BUFFER, index*sizeof(float), sizeof(float), &scale);
}

Particle::~Particle()
{
}

void Particle::setColor(float r, float g, float b) { 
	color << std::min(r, 1.0f), std::min(g, 1.0f), std::min(b, 1.0f); 
};

void Particle::rebirth()
{
	m = 1.0f;
	alpha = 1.0f;
	tEnd = tGlobal + lifespan;
	
	d = randFloat(0.0f, 1.0f);
	x << randFloat(-0.1f, 0.1f), randFloat(-0.1f, 0.1f), randFloat(-0.1f, 0.1f);

	speed = randFloat(MIN_PARTICLE_SPEED, MAX_PARTICLE_SPEED);
	dir << randFloat(-1.0f, 1.0f), randFloat(-1.0f, 1.0f), randFloat(-1.0f, 1.0f);
	dir.normalize();
}

void Particle::rebirth(Eigen::Vector3f &basePos, Eigen::Vector3f &dirMin, Eigen::Vector3f &dirMax, float &speedMin, float &speedMax, float &lifespan)
{
	m = 1.0f;
	alpha = 1.0f;
	tEnd = tGlobal + lifespan;
	
	// Gravity towards origin
	d = randFloat(0.0f, 1.0f);
	x = basePos + Eigen::Vector3f(randFloat(-0.1f, 0.1f), randFloat(-0.1f, 0.1f), randFloat(-0.1f, 0.1f));

	speed = randFloat(speedMin, speedMax);
	dir << randFloat(dirMin.x(), dirMax.x()), randFloat(dirMin.y(), dirMax.y()), randFloat(dirMin.z(), dirMax.z());
	dir.normalize();
}


void Particle::step()
{
	if (tGlobal > tEnd){
		rebirth();	
	}

	// Update alpha based on current time
	alpha = (tEnd - tGlobal)/lifespan;

	speed *= PARTICLE_DECELERATION;
	x += speed * dir;
}

void Particle::step(Eigen::Vector3f &basePos, Eigen::Vector3f &dirMin, Eigen::Vector3f &dirMax, float &speedMin, float &speedMax, float &lifespan)
{
	// Update alpha based on current time
	alpha = (tEnd - tGlobal)/lifespan;

	speed *= PARTICLE_DECELERATION;
	x += speed * dir;
}

float Particle::randFloat(float l, float h)
{
	float r = rand() / (float)RAND_MAX;
	return (1.0f - r) * l + r * h;
}

float Particle::percentageLived(){

	if (tGlobal > tEnd){
		return 1.0f;
	}

	float per = ((float)(tEnd - tGlobal) / lifespan);

	return std::min(per, 1.0f);
}
