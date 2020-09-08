#pragma once

#include "trigger.h"

#include "01_platform/wci.h"  // Platform's WCI and gamePlatform defined here, 
                              // to use 'BITMASK_READY', 'BITMASK_FIRE' for validating trigger
// #include <functional>

// These values are supposed to be less than 255 in most cases,
// so 'unsigned char' could be used as index to address
// the elements in an array[these_number]
#define MAX_NUM_PLANE         (16)
#define MAX_NUM_OBSTACLE_CUBE (32)
#define MAX_NUM_TRIGGERS      (32)
// #define MAX_NUM_TRANSFERS  (16)

#define ZHEIGHT_OBSTACLE_CUBE (4.0f)
#define ZHEIGHT_TRIGGER_PAD   (0.2f)
// #define ZHEIGHT_LIFT_PAD   (0.4f)

// #define COLLISION_INFO_BIT_MASK_COLLISION_VALID  (1 << (7))
// #define COLLISION_INFO_BIT_MASK_COLLISION_VALID  (1 << (sizeof(unsigned char) * 8 - 1))
// #define COLLISION_INFO_BIT_MASK_NORMAL_NORTH     (1 << (0))
// #define COLLISION_INFO_BIT_MASK_NORMAL_EAST      (1 << (1))
// #define COLLISION_INFO_BIT_MASK_NORMAL_SOUTH     (1 << (2))
// #define COLLISION_INFO_BIT_MASK_NORMAL_WEST      (1 << (3))
// #define COLLISION_INFO_BIT_MASK_INSIDE_RECT      (1 << (4))
#define COLLISION_BIT_MASK_VALID            (1 << (3))  // 0b: 1000
#define COLLISION_BIT_MASK_INSIDE_RECT      (1 << (2))  // 0b: 0100

#define COLLISION_BIT_MASK_CLEAR_STATUS     (3 << (0))  // 0b: 0011
#define COLLISION_STATUS_NORMAL_NORTH       (0 << (0))  // 0b: 0000
#define COLLISION_STATUS_NORMAL_EAST        (1 << (0))  // 0b: 0001
#define COLLISION_STATUS_NORMAL_SOUTH       (2 << (0))  // 0b: 0010
#define COLLISION_STATUS_NORMAL_WEST        (3 << (0))  // 0b: 0011


#define SCENE_STATUS_PLAYING_OPEN_DOOR_ANIMATION   (1 << (0))  // set/cleared with open-door trigger's update()
#define SCENE_STATUS_ALLOCATING_VBO_AND_TEXTURE    (1 << (1))  // set after open-door set, cleared in testLayer
#define SCENE_STATUS_UPDATE_PLAYER_BY_INPUT        (1 << (2))
#define SCENE_STATUS_UPDATE_PLAYER_BY_CHANGE_PLANE (1 << (3))
#define SCENE_STATUS_CHANGE_PLANE_INDEX            (1 << (4))

#define __PI     (3.14f)
#define __PI_2   (1.57f)
#define __2_PI   (6.28f)
#define __PI_4   (0.785f)
#define __3_PI_4 (2.355f)


typedef enum {
    roomInit, thePorch, theHall, theEastRoom, theWestRoom, theYard, roomMax
} room;

// struct transfer {
//     cube pad;

//     // Data for trigger_ChangePlane  ==> changed tobe struct 'transfer'
//     unsigned short transAnimTtime;   // use animation (and script ???) to control how the player
//     unsigned short transAnimRepeat;  // gets from 1 plane to another (eg. get up/down stairs)
//     glm::vec3 transVector;
//     float transSpeed;
//     float validAngle;
//     unsigned char targetPlaneIdx;

//     unsigned char status;
//     // glm::vec3 *targetPos; passed in when updater called
// };

// room info from Open-Door trigger
struct targetPlaceInfo {
    room rm;
    unsigned char pl;
    float enteringPosRot[4];
};

// position/plane-idx info from change-plane trigger
struct targetPlaneInfo {
    float pos[3];
    unsigned char plIdx;
};

struct scenePlane {
    cube obsCube[MAX_NUM_OBSTACLE_CUBE];
    unsigned char numObsCubes;

    triggerPad trigger[MAX_NUM_TRIGGERS];
    unsigned char numTriggers;

    // transfer lift[MAX_NUM_TRANSFERS];
    // unsigned char numLifts;
};


void scene_setStatus(unsigned char bitMask, unsigned char set);
void scene_setStatus(unsigned char sts);
unsigned char scene_getStatus(unsigned char bitMask);
unsigned char scene_getStatus();

void scene_setCurrentPlane(unsigned char i);
scenePlane *scene_getCurrentPlane();

targetPlaceInfo *scene_getTargetPlace();
targetPlaneInfo *scene_getTargetPositionPlane();


// unsigned char scene_findValidTriggerPads(glm::vec3 *pos, float *rot, unsigned char input /* float delta */);
// unsigned char scene_updateValidTriggerPads(float delta);
void scene_updateTriggerAndStatus(float delta, glm::vec3 *pos, float *rot, unsigned char input);

void scene_LoadData(room r, unsigned char plane);

unsigned char scene_findCollidingObsCube(float delta, glm::vec3 desPos);
glm::vec3 scene_normalFromCollidingObsCube();
unsigned char scene_insideCollidingObsCube(glm::vec3 desPos /* float delta */);
unsigned char scene_insideAnyObsCube(glm::vec3 desPos);
