#pragma once

#include "01_platform/wci.h"  // to use definition for width/height

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
    #include <glm/gtc/type_ptr.hpp>
using namespace glm;

void updateMatricesForCamera(glm::vec3 *camPos, float *camHvAngle);
void computeMatricesFromInputs();

glm::mat4 getViewMatrix();
glm::mat4 getProjectionMatrix();
