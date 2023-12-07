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
	scale = randFloat(5.0f, 8.0f);

	tEnd = tGlobal + PARTICLE_LIFESPAN;

	rebirth();
	
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

void Particle::rebirth()
{
	m = 1.0f;
	alpha = 1.0f;
	
	// Gravity towards origin
	d = randFloat(0.0f, 1.0f);
	x << randFloat(-0.1f, 0.1f), randFloat(-0.1f, 0.1f), randFloat(-0.1f, 0.1f);
	v << randFloat(-1.0f, 1.0f), randFloat(-1.0f, 1.0f), randFloat(-1.0f, 1.0f);

	tEnd = tGlobal + PARTICLE_LIFESPAN;
}

void Particle::step(float t, float h, const Vector3f &g)
{
	// Update alpha based on current time
	alpha = (tEnd-t)/PARTICLE_LIFESPAN;
	
	//
	// <IMPLEMENT ME>
	// Accumulate forces and update velocity
	//
	
	// Update position
	x += h*v;
}

float Particle::randFloat(float l, float h)
{
	float r = rand() / (float)RAND_MAX;
	return (1.0f - r) * l + r * h;
}