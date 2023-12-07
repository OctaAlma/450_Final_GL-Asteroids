#pragma once
#ifndef _PARTICLE_H_
#define _PARTICLE_H_

#define _USE_MATH_DEFINES
#include <memory>
#include <vector>

#define GLEW_STATIC
#include <GL/glew.h>

#include <Eigen/Dense>

#define PARTICLE_LIFESPAN 1.0

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
	void step(float t, float h, const Eigen::Vector3f &g);
	
	static float randFloat(float l, float h);
	
private:
	// Properties that are fixed
	Eigen::Map<Eigen::Vector3f> color; // color (mapped to a location in colBuf)
	float &scale;                      // size (mapped to a location in scaBuf)
	
	// Properties that changes every rebirth
	float m;        // mass
	float d;        // viscous damping
	float lifespan; // how long this particle lives
	float tEnd;     // time this particle dies
	
	// Properties that changes every frame
	Eigen::Map<Eigen::Vector3f> x; // position (mapped to a location in posBuf)
	Eigen::Vector3f v;             // velocity
	float &alpha;                  // mapped to a location in alpBuf
};

#endif
