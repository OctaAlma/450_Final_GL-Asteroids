#pragma once
#ifndef _PARTICLE_H_
#define _PARTICLE_H_

#define _USE_MATH_DEFINES
#include <memory>
#include <vector>

#define GLEW_STATIC
#include <GL/glew.h>

#include <Eigen/Dense>

#define MIN_PARTICLE_SPEED 1.0f
#define MAX_PARTICLE_SPEED 1.5f

#define MIN_PARTICLE_LIFESPAN 1.0
#define MAX_PARTICLE_LIFESPAN 2.0

#define PARTICLE_LIFESPAN 1.0
#define PARTICLE_DECELERATION 0.9f

#define MIN_PARTICLE_SIZE 3.0f
#define MAX_PARTICLE_SIZE 5.0f

extern double tGlobal;

class MatrixStack;
class Program;
class Texture;

class Particle
{
public:
	
	Particle(int index, GLuint colBufID, GLuint scaBufID, std::vector<float> &posBuf, std::vector<float> &colBuf, 
		std::vector<float> &alpBuf, std::vector<float> &scaBuf, Eigen::Vector3f col);
	
	virtual ~Particle();
	
	void rebirth();
	void rebirth(Eigen::Vector3f &basePos, Eigen::Vector3f &dirMin, Eigen::Vector3f &dirMax, float &speedMin, float &speedMax, float &lifespan);
	
	void step();
	void step(Eigen::Vector3f &basePos, Eigen::Vector3f &dirMin, Eigen::Vector3f &dirMax, float &speedMin, float &speedMax, float &lifespan);

	void setLifespan(float ls) { this->lifespan = ls; }
	float getLifespan() { return this->lifespan; }
	bool isAlive() { return this->tEnd > tGlobal; }
	float percentageLived();
	
	Eigen::Vector3f getPos() { return Eigen::Vector3f(x[0], x[1], x[2]); }
	Eigen::Vector3f getColor() { return Eigen::Vector3f(color[0], color[1], color[2]); }

	void setColor(float r, float g, float b);
	
	static float randFloat(float l, float h);
	
private:
	// Properties that are fixed
	Eigen::Map<Eigen::Vector3f> color; // color (mapped to a location in colBuf)
	float &scale;                      // size (mapped to a location in scaBuf)
	
	// Properties that changes every rebirth
	float m;        // mass
	float d;        // viscous damping
	float tEnd = 0.0f;     // time this particle dies
	float lifespan = 1.0f;
	
	// Properties that changes every frame
	Eigen::Map<Eigen::Vector3f> x; // position (mapped to a location in posBuf)
	// Eigen::Vector3f v;             // velocity
	Eigen::Vector3f dir;             // direction
	float speed; 

	float &alpha;                  // mapped to a location in alpBuf
};

#endif
