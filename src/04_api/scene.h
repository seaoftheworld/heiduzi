#pragma once

#include "trigger.h"
// #include <functional>

// These values are supposed to be less than 255 in most cases,
// so 'unsigned char' could be used as index the address
// the elements in an array[these_number]
#define MAX_NUM_PLANE         (16)
#define MAX_NUM_OBSTACLE_CUBE (32)
#define MAX_NUM_TRIGGERS      (32)

#define ZHEIGHT_OBSTACLE_CUBE (4.0f)
#define ZHEIGHT_TRIGGER_PAD   (0.2f)

// #define COLLISION_INFO_BIT_MASK_COLLISION_VALID  (1 << (7))
#define COLLISION_INFO_BIT_MASK_COLLISION_VALID  (1 << (sizeof(unsigned char) * 8 - 1))
#define COLLISION_INFO_BIT_MASK_NORMAL_NORTH  (1 << (0))
#define COLLISION_INFO_BIT_MASK_NORMAL_EAST   (1 << (1))
#define COLLISION_INFO_BIT_MASK_NORMAL_SOUTH  (1 << (2))
#define COLLISION_INFO_BIT_MASK_NORMAL_WEST   (1 << (3))
#define COLLISION_INFO_BIT_MASK_INSIDE_RECT   (1 << (4))

typedef enum {
    thePorch, theHall, theEastRoom, theWestRoom, theYard, roomMax
} room;

struct scenePlane {
    cube obsCube[MAX_NUM_OBSTACLE_CUBE];
    unsigned char numObsCubes;

    triggerPad trigger[MAX_NUM_TRIGGERS];
    unsigned char numTriggers;
    
    // Other rects, triggers
    //... ...
};


scenePlane *scene_GetCurrentPlane();
void scene_SetCurrentPlane(unsigned char i);

void scene_LoadData(room r);

unsigned char scene_InObsCube(float delta, glm::vec3 destinationPosition);
unsigned char scene_NearObsCube(float delta, glm::vec3 destinationPosition);
glm::vec3 scene_NormalFromObsCube();

// glm::vec2 coordConvert2d(glm::vec2 coord, glm::vec2 trans, float r);
// glm::vec3 sceneComboNormalFromObsCube();



/*
struct triggerPad {
    triggerType type;
    cube pad;

    // Could also use a pointer-to-function if the functions to be called are not member-functions.
    // When the functions tobe called are member-functions, however, pointer-to-function could also be used,
    // it can be used with the object of that class together when calling:
    // void Class::*pf(int, int); Class obj;
    // (obj.*pf)(parameters ...);
    std::function<void(triggerPad *, float)> update;
};
//*/
