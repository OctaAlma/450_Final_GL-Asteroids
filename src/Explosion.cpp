#include "Explosion.h"

#include <iostream>
using std::cout, std::endl;

Explosion::Explosion(std::string RESOURCE_DIR, Eigen::Vector3f col){
    posBuf.resize(3 * NUM_PARTICLES_PER_EXPLOSION);
	colBuf.resize(3 * NUM_PARTICLES_PER_EXPLOSION);
	alpBuf.resize(NUM_PARTICLES_PER_EXPLOSION);
	scaBuf.resize(NUM_PARTICLES_PER_EXPLOSION);
	tCreated = tGlobal;
	
	for(int i = 0; i < NUM_PARTICLES_PER_EXPLOSION; ++i) {
		posBuf[3*i+0] = 0.0f;
		posBuf[3*i+1] = 0.0f;
		posBuf[3*i+2] = 0.0f;
		colBuf[3*i+0] = 1.0f;
		colBuf[3*i+1] = 1.0f;
		colBuf[3*i+2] = 1.0f;
		alpBuf[i] = 1.0f;
		scaBuf[i] = 1.0f;
	}

	// Generate buffer IDs
	GLuint bufs[4];
	glGenBuffers(4, bufs);
	posBufID = bufs[0];
	colBufID = bufs[1];
	alpBufID = bufs[2];
	scaBufID = bufs[3];
	
	// Send color buffer to GPU
	glBindBuffer(GL_ARRAY_BUFFER, colBufID);
	glBufferData(GL_ARRAY_BUFFER, colBuf.size()*sizeof(float), &colBuf[0], GL_STATIC_DRAW);
	
	// Send scale buffer to GPU
	glBindBuffer(GL_ARRAY_BUFFER, scaBufID);
	glBufferData(GL_ARRAY_BUFFER, scaBuf.size()*sizeof(float), &scaBuf[0], GL_STATIC_DRAW);
	
	assert(glGetError() == GL_NO_ERROR);

    for(int i = 0; i < NUM_PARTICLES_PER_EXPLOSION; ++i) {
		auto p = std::make_shared<Particle>(i, colBufID, scaBufID, posBuf, colBuf, alpBuf, scaBuf, col);
		particles.push_back(p);
 		p->rebirth();
	}
}

void Explosion::setCenter(glm::vec3 c){
    center = c;
}

void Explosion::step(){
    // TO-DO: What do the particles do as time progresses?
	float h = 1.0f;
	Eigen::Vector3f grav;
	grav << 0.0f, 9.81f, 0.0f;

    // This can be parallelized!
    for(int i = 0; i < (int)particles.size(); ++i) {
        particles[i]->step(tGlobal, h, grav);
    }
}

void Explosion::draw(std::shared_ptr<MatrixStack> &P, 
    std::shared_ptr<MatrixStack> &MV, int width, int height, 
    std::shared_ptr<Texture> &alphaTex, std::shared_ptr<Program> &prog)
{
    glEnable(GL_BLEND);
	glDepthMask(GL_FALSE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	prog->bind();
	alphaTex->bind(prog->getUniform("alphaTexture"));

    MV->pushMatrix();
    MV->translate(center);
	
	glUniformMatrix4fv(prog->getUniform("P"), 1, GL_FALSE, glm::value_ptr(P->topMatrix()));
	glUniformMatrix4fv(prog->getUniform("MV"), 1, GL_FALSE, glm::value_ptr(MV->topMatrix()));
	glUniform2f(prog->getUniform("screenSize"), (float)width, (float)height);    
    drawParticles(prog);

    MV->popMatrix();

	alphaTex->unbind();
	prog->unbind();
	
	glDepthMask(GL_TRUE);
	glDisable(GL_BLEND);
}

void Explosion::drawParticles(std::shared_ptr<Program> &prog)
{
    // Enable, bind, and send position array
	glEnableVertexAttribArray(prog->getAttribute("aPos"));
	glBindBuffer(GL_ARRAY_BUFFER, posBufID);
	glBufferData(GL_ARRAY_BUFFER, posBuf.size()*sizeof(float), &posBuf[0], GL_DYNAMIC_DRAW);
	glVertexAttribPointer(prog->getAttribute("aPos"), 3, GL_FLOAT, GL_FALSE, 0, 0);
	
	// Enable, bind, and send alpha array
	glEnableVertexAttribArray(prog->getAttribute("aAlp"));
	glBindBuffer(GL_ARRAY_BUFFER, alpBufID);
	glBufferData(GL_ARRAY_BUFFER, alpBuf.size()*sizeof(float), &alpBuf[0], GL_DYNAMIC_DRAW);
	glVertexAttribPointer(prog->getAttribute("aAlp"), 1, GL_FLOAT, GL_FALSE, 0, 0);
	
	// Enable and bind color array
	glEnableVertexAttribArray(prog->getAttribute("aCol"));
	glBindBuffer(GL_ARRAY_BUFFER, colBufID);
	glVertexAttribPointer(prog->getAttribute("aCol"), 3, GL_FLOAT, GL_FALSE, 0, 0);
	
	// Enable and bind scale array
	glEnableVertexAttribArray(prog->getAttribute("aSca"));
	glBindBuffer(GL_ARRAY_BUFFER, scaBufID);
	glVertexAttribPointer(prog->getAttribute("aSca"), 1, GL_FLOAT, GL_FALSE, 0, 0);
	
	// Draw
	glDrawArrays(GL_POINTS, 0, 3*particles.size());
	
	// Disable and unbind
	glDisableVertexAttribArray(prog->getAttribute("aSca"));
	glDisableVertexAttribArray(prog->getAttribute("aCol"));
	glDisableVertexAttribArray(prog->getAttribute("aAlp"));
	glDisableVertexAttribArray(prog->getAttribute("aPos"));
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}