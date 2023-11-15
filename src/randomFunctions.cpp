#include "randomFunctions.h"
#include <cmath>
// The following was obtained from:
// https://stackoverflow.com/questions/5289613/generate-random-float-between-two-floats
float randomFloat(float a, float b) {
    float random = ((float) rand()) / (float) RAND_MAX;
    float diff = b - a;
    float r = random * diff;
    return a + r;
}