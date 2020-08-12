#pragma once

#include "01_platform/wci.h"  // to use definition for width/height

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
    #include <glm/gtc/type_ptr.hpp>
using namespace glm;

void computeMatricesFromInputs();
glm::mat4 getViewMatrix();
glm::mat4 getProjectionMatrix();
