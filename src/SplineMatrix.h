#pragma once
#ifndef SPLINEMATRIX
#define SPLINEMATRIX

#include <iostream>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>

glm::mat4 createBezier();
glm::mat4 createCatmull();
glm::mat4 createBspline();

#endif