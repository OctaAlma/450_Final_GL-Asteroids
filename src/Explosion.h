#pragma once
#ifndef EXPLOSION_H
#define EXPLOSION_H

#include "Particle.h"
#include "Program.h"
#include "Texture.h"
#include "MatrixStack.h"

#define GLEW_STATIC
#include <GL/glew.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <memory>
#include <vector>

#define NUM_PARTICLES_PER_EXPLOSION 1000
#define EXPLOSION_LIFESPAN 1.0 // In seconds

extern double tGlobal;

class Explosion {
public:
    Explosion(){}
    Explosion(std::string RESOURCE_DIR, Eigen::Vector3f col);
    void step();
    void draw(std::shared_ptr<MatrixStack> &P, std::shared_ptr<MatrixStack> &MV, int width, int height, 
            std::shared_ptr<Texture> &alphaTex, std::shared_ptr<Program> &prog);
    void drawParticles(std::shared_ptr<Program> &prog);
    void setCenter(glm::vec3 c);

    bool isAlive() { return tGlobal < (tCreated + EXPLOSION_LIFESPAN); }

protected:
    double tCreated = 0.0f;
    glm::vec3 center;
    std::vector< std::shared_ptr< Particle> > particles;

    std::vector<float> posBuf;
    std::vector<float> colBuf;
    std::vector<float> alpBuf;
    std::vector<float> scaBuf;

    GLuint posBufID;
    GLuint colBufID;
    GLuint alpBufID;
    GLuint scaBufID;

    void sendColorBuf();
    void sendScaleBuf();
};

#endif