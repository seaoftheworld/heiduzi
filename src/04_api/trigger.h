#pragma once

// Include 'GLM' to only use 'glm::vec3' ... data type, 
// and glm::translate(), rotate(), normalize() ...
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// #define NUM_VERTICES_FOR_RECT (4)
#define NUM_VERTICES_FOR_CUBE (8)

#define TRIGGER_STATUS_UPDATING          (1 << (sizeof(unsigned char) * 8 - 1))
#define TRIGGER_STATUS_FIRST_UPDATE_DONE (1 << (6))
#define TRIGGER_STATUS_LAST_UPDATE_DONE  (1 << (5))
// #define TRIGGER_STATUS_POS_VALID    (1 << (0))
// #define TRIGGER_STATUS_ANG_VALID    (1 << (1))
// #define TRIGGER_STATUS_INPUT_VALID  (1 << (2))

#define TRIGGER_TYPE_CHANGE_PLANE "trigger_ChangePlane"
#define TRIGGER_TYPE_CHANGE_VIEW "trigger_ChangeVIEW"
#define TRIGGER_TYPE_OPEN_DOOR "trigger_OpenDoor"

enum nIdx {
    north = 0, east, south, west, max
};

struct rect {
    glm::vec3 leftDown = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 center = glm::vec3(0.0f, 0.0f, 0.0f);
    
    float width = 0, height = 0;
    float rotZ = 0;

    glm::vec3 normal[nIdx::max] = { glm::vec3(0.0f) };
    unsigned char collisionInfo = 0;

    rect(glm::vec3 ctr, glm::vec3 lb, float *size_rot);
    rect() { /* printf("  __rect()"); */ }
};

struct cube {
    rect bottomRect;
    float height;

    //    7----5      7----5
    //   /|   /|     /    /|
    //  1----3 |    1----3 |
    //  | |  | |    |    | |
    //  | 6--|-4    |    | 4
    //  |/   |/     |    |/
    //  0----2      0----2
    //
    glm::vec3 vertices[NUM_VERTICES_FOR_CUBE] = { glm::vec3(0.0f) };

    cube(glm::vec3 rectCenter, glm::vec3 rectLeftBottom, float *size_rot, float cubeHeight);
    cube() { /* printf("  __cube()"); */ }
};

// 'triggerUpdater' Class forward declaration
class triggerUpdater;

enum triggerType {
    trigger_None = 0,
    trigger_Investigate,
    trigger_Item,
    trigger_Ammo,
    trigger_OpenDoor,
    trigger_ChangeView,
    trigger_ChangePlane,
    trigger_Slope,
    trigger_Max
};

// triggerPad Class
class triggerPad {
public:
    triggerType type;
    cube pad; 

    unsigned char status;

    float valid_angle;
    float anim_time;             // can be used for changePlane-trigger (go up/down stairs),  
    unsigned short anim_repeat;  // pick item trigger (squad to pick), or open door anim...

    glm::vec3 *pos;
    float *rot;

    // Data for trigger_Investigate
    // Data for trigger_Item
    // Data for trigger_Ammo
    
    // Data only used for trigger_ChangeView
    // glm::vec3 vector;
    // float speed;
    float movingVecSpd[4];
    unsigned char targetPlaneIdx;

    // Data only used for trigger_OpenDoor
    float enteringPosRot[4];
    unsigned char targetRoomIdx;
    
    // Data for trigger_Slope

    void init(triggerUpdater *p);

    // unsigned char isTriggered();
    // unsigned char calculateStatus(glm::vec3 pos, float r);
    // unsigned char getStatus();

    void update(float delta);

private:
    triggerUpdater *updater;
};

// triggerUpdater Class and Objects (object's address tobe used as init()'s input parameter)
class triggerUpdater {
public:
    virtual ~triggerUpdater() {}
    virtual void update(triggerPad *trigger, float delta) = 0;
};

// implementations for different types of triggers
class changeViewUpdater : public triggerUpdater {
    void update(triggerPad *trigger, float delta) override;
};
class changePlaneUpdater : public triggerUpdater {
    void update(triggerPad *trigger, float delta) override;
};
class openDoorUpdater : public triggerUpdater {
    void update(triggerPad *trigger, float delta) override;
};
// class ...Updater : public triggerUpdater { ... };

extern changeViewUpdater updater_ChangeView;
extern changePlaneUpdater updater_ChangePlane;
extern openDoorUpdater updater_OpenDoor;
// extern ...Updater updater_...;

unsigned char insideArea(glm::vec3 *pos, rect *area, float margin, glm::vec2 *converted);
