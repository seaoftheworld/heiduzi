#include "actor.h"
#define PI__ (3.14f)

static glm::vec3 moveForwardBackward(unsigned char forward, actor *act, float deltaTime) {
    if (!act) return glm::vec3(0.0f, 0.0f, 0.0f); // assert
    
    float step = (forward) ? 
        ( deltaTime * act->speed[actor::speedIdx::forward] ) : 
        ( deltaTime * act->speed[actor::speedIdx::backward] );

    // sin(), cos() calls are from:
    //    E:\Windows Kits\10\Include\10.0.18362.0\ucrt\corecrt_math.h
    // some lib required for msvc++ ???
    glm::vec3 deltaXY = (forward) ? 
        ( glm::vec3(-sin(act->yAngle) * step,  cos(act->yAngle) * step, 0.0) ) : 
        ( glm::vec3( sin(act->yAngle) * step, -cos(act->yAngle) * step, 0.0) );

    // act->position += deltaXY;
    
    // To be modified to use OOB-box on actor for detection, 
    // rather than using a point like for now.
    // act->position = obstacleCorrection(act->position + deltaXY, act->position);

    return deltaXY;
}

static void turnLeftRight(unsigned char left, actor *act, float deltaTime) {
    if (!act) return;  // assert

    float step = deltaTime * act->speed[actor::speedIdx::turn];
    act->yAngle = (left) ? (act->yAngle + step) : (act->yAngle - step);
    
    if (act->yAngle > PI__) {
        act->yAngle -= (PI__ + PI__);  // 2 * PI__;
    }
    if (act->yAngle <= -PI__ ) {
        act->yAngle += (PI__ + PI__);  // 2 * PI__;
    }
}

// void moveForward(actor *act, float deltaTime) {
glm::vec3 moveForward(actor *act, float deltaTime) {
    return moveForwardBackward(1, act, deltaTime);
}

// void moveBackward(actor *act, float deltaTime) {
glm::vec3 moveBackward(actor *act, float deltaTime) {
    return moveForwardBackward(0, act, deltaTime);
}

void turnLeft(actor *act, float deltaTime) {
    turnLeftRight(1, act, deltaTime);
}

void turnRight(actor *act, float deltaTime) {
    turnLeftRight(0, act, deltaTime);
}
