#pragma once

// Include 'GLM' to only use 'glm::vec3' ... data type for calculating 
// the position of an actor (pos_vec3 + deltaXY_vec3)
#include <glm/glm.hpp>

struct actor {
    enum speedIdx { 
        forward = 0, backward, turn, max
    };
    
    glm::vec3 position;
    float yAngle;
    float speed[max];

    actor(glm::vec3 pos, float angle, float *speeds) {
        position = pos;
        yAngle = angle;
        for (unsigned int i = forward; i < max; i++) {
            speed[i] = speeds[i];
        }
    }
};

// void moveForward(actor *act, float deltaTime);
// void moveBackward(actor *act, float deltaTime);
glm::vec3 moveForward(actor *act, float deltaTime);
glm::vec3 moveBackward(actor *act, float deltaTime);

void turnLeft(actor *act, float deltaTime);
void turnRight(actor *act, float deltaTime);
