#pragma once

// Include 'GLM' to only use 'glm::vec3' ... data type, 
// and glm::translate(), rotate(), normalize() ...
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// #define NUM_VERTICES_FOR_RECT (4)
#define NUM_VERTICES_FOR_CUBE (8)

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


    // Data for trigger_Investigate
    // Data for trigger_Item
    // Data for trigger_Ammo
    // Data for trigger_OpenDoor
    // Data for trigger_ChangeView

    // Data for trigger_ChangePlane
    glm::vec3 vector;
    float speed;
    unsigned short anim_time;    // use animation or script (???) to control how the player
    unsigned short anim_repeat;  // gets from 1 plane to another (eg. get up/down stairs)
    float valid_yAngle;
    
    // Data for trigger_Slope

    void init(triggerUpdater* p);

    unsigned char isTriggered();
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
// class ...Updater : public triggerUpdater { ... };

extern changeViewUpdater updater_ChangeView;
extern changePlaneUpdater updater_ChangePlane;
// extern ...Updater updater_...;
