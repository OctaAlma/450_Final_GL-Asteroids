#include "ExhaustFire.h"
#include "MatrixStack.h"
#include <iostream>
using std::cout, std::endl;

#define EXHAUST_X_OFFSET 0.5f
#define EXHAUST_Y_OFFSET 0.75f
#define EXHAUST_Z_OFFSET -1.0f
#define NUM_EXHAUST_PARTICLES 10000

Eigen::Vector3f dirMin(-0.2f, -0.3f, -0.5f);
Eigen::Vector3f dirMax(0.2f, 0.3f, -1.0f);
float speedMin = 0.1f;
float speedMax = 0.2f;
float minls = 0.25f;
float maxls = 0.5f;

ExhaustFire::ExhaustFire(const std::string RESOURCE_DIR, int e)
{
	exhaust = e;

    Eigen::Vector3f col(0.7f, 0.0f, 0.0f);

    posBuf.resize(3 * NUM_EXHAUST_PARTICLES);
	colBuf.resize(3 * NUM_EXHAUST_PARTICLES);
	alpBuf.resize(NUM_EXHAUST_PARTICLES);
	scaBuf.resize(NUM_EXHAUST_PARTICLES);
	tCreated = tGlobal;
	
	for(int i = 0; i < NUM_EXHAUST_PARTICLES; ++i) {
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

	Eigen::Vector3f c(center.x, center.y, center.z);

    for(int i = 0; i < NUM_EXHAUST_PARTICLES; ++i) {
		auto p = std::make_shared<Particle>(i, colBufID, scaBufID, posBuf, colBuf, alpBuf, scaBuf, col);
		p->setLifespan(Particle::randFloat(minls, maxls));
		particles.push_back(p);
        
	}

	// Send color buffer to GPU
	glBindBuffer(GL_ARRAY_BUFFER, colBufID);
	glBufferData(GL_ARRAY_BUFFER, colBuf.size()*sizeof(float), &colBuf[0], GL_STATIC_DRAW);
	
	// Send scale buffer to GPU
	glBindBuffer(GL_ARRAY_BUFFER, scaBufID);
	glBufferData(GL_ARRAY_BUFFER, scaBuf.size()*sizeof(float), &scaBuf[0], GL_STATIC_DRAW);
}

void printVec(std::vector<float> v){
	for (int i = 0; i < v.size(); i++){
		cout << v.at(i) << ",";
	}
	cout << endl << endl;
}

float angleBetweenVecs(glm::vec3 v1, glm::vec3 v2)
{
	float n = glm::length(v1) * glm::length(v2);
	
	if (n == 0.0f){
		return 0.0f;
	}
	
	return acos( glm::dot(v1, v2) / (glm::length(v1) * glm::length(v2)));
}

// Left (EXHAUST_X_OFFSET, EXHAUST_Y_OFFSET, EXHAUST_Z_OFFSET)
// right (-EXHAUST_X_OFFSET, EXHAUST_Y_OFFSET, EXHAUST_Z_OFFSET)
void ExhaustFire::step(MatrixStack M, bool wPressed)
{
	M.rotate(-roll, 0, 0, 1.0f);

	if (exhaust == LEFT){
		M.translate(-EXHAUST_X_OFFSET, EXHAUST_Y_OFFSET, EXHAUST_Z_OFFSET);
	}else{
		M.translate(EXHAUST_X_OFFSET, EXHAUST_Y_OFFSET, EXHAUST_Z_OFFSET);
	}

	glm::vec3 startPos = M.topMatrix() * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

	Eigen::Vector3f p(startPos.x, startPos.y, startPos.z);

    for(int i = 0; i < (int)particles.size(); ++i) {
        float ls = Particle::randFloat(minls, maxls);

        if (wPressed && !particles[i]->isAlive()){
			particles[i]->rebirth(p, dirMin, dirMax, speedMin, speedMax, ls);
			continue;
        }

        particles[i]->step(p, dirMin, dirMax, speedMin, speedMax, ls);
		float lived = 1.0f - particles[i]->percentageLived();
        particles[i]->setColor(0.7f, 0.0f + lived / 2.0f, lived / 4.0f);
    }

	sendColorBuf();
}

void ExhaustFire::draw(std::shared_ptr<MatrixStack> &P, 
    std::shared_ptr<MatrixStack> &MV, int width, int height, 
    std::shared_ptr<Texture> &alphaTex, std::shared_ptr<Program> &prog)
{
    glEnable(GL_BLEND);
	glDepthMask(GL_FALSE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	alphaTex->bind(prog->getUniform("alphaTexture"));

    MV->pushMatrix();
	
	glUniformMatrix4fv(prog->getUniform("P"), 1, GL_FALSE, glm::value_ptr(P->topMatrix()));
	glUniformMatrix4fv(prog->getUniform("MV"), 1, GL_FALSE, glm::value_ptr(MV->topMatrix()));
	glUniform2f(prog->getUniform("screenSize"), (float)width, (float)height);    
    drawParticles(prog);

    MV->popMatrix();

	alphaTex->unbind();
	
	glDepthMask(GL_TRUE);
	glDisable(GL_BLEND);
}

void ExhaustFire::sendColorBuf(){
	// Send color buffer to GPU
	glBindBuffer(GL_ARRAY_BUFFER, colBufID);
	glBufferData(GL_ARRAY_BUFFER, colBuf.size()*sizeof(float), &colBuf[0], GL_STATIC_DRAW);
}

