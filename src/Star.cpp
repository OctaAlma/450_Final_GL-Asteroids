#include "Star.h"
#include "randomFunctions.h"

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

Star::Star(){
    this->pos = glm::vec3(rand(), rand(), rand());

    bool negX = rand() % 2;
    bool negY = rand() % 2;
    bool negZ = rand() % 2;

    if (negX){ pos.x *= -1.0f; }
    if (negY){ pos.y *= -1.0f; }
    if (negZ){ pos.z *= -1.0f; }
    
    this->pos = STAR_RADIUS * glm::normalize(pos);

    this->size = randomFloat(MIN_STAR_SIZE, MAX_STAR_SIZE);
}

void Star::draw(){
    glPointSize(size);
    glColor3f(1, 1, 1);
    
    glBegin(GL_POINTS);
    glVertex3f(pos.x, pos.y, pos.z);
    glEnd();
}