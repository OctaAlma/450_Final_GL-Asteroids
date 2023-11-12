#include "SplineMatrix.h"

glm::mat4 createBezier(){
	glm::mat4 matBezier;
	// note: mat[col][row]
	// bezier curve matrix
	glm::vec4 col1(1,0,0,0);
	glm::vec4 col2(-3,3,0,0);
	glm::vec4 col3(3,-6,3,0);
	glm::vec4 col4(-1,3,-3,1);

	matBezier[0] = col1;
	matBezier[1] = col2;
	matBezier[2] = col3;
	matBezier[3] = col4;

	return matBezier;
}

glm::mat4 createCatmull(){
	glm::mat4 matCatmull;
	// note: mat[col][row]
	// bezier curve matrix
	glm::vec4 col1(0,2,0,0);
	glm::vec4 col2(-1,0,1,0);
	glm::vec4 col3(2,-5,4,-1);
	glm::vec4 col4(-1,3,-3,1);

	matCatmull[0] = col1;
	matCatmull[1] = col2;
	matCatmull[2] = col3;
	matCatmull[3] = col4;

	matCatmull = 1.0f/2.0f * matCatmull;

	return matCatmull;
}

glm::mat4 createBspline(){
	glm::mat4 matBspline;

	// note: mat[col][row]
	// bezier curve matrix
	glm::vec4 col1(1,4,1,0);
	glm::vec4 col2(-3,0,3,0);
	glm::vec4 col3(3,-6,3,0);
	glm::vec4 col4(-1,3,-3,1);

	matBspline[0] = col1;
	matBspline[1] = col2;
	matBspline[2] = col3;
	matBspline[3] = col4;

	matBspline = 1.0f/6.0f * matBspline;

	return matBspline;
}