#include "trigger.h"
#include <stdio.h>

changeViewUpdater updater_ChangeView;
changePlaneUpdater updater_ChangePlane;
   openDoorUpdater updater_OpenDoor;
// ...Updater updater_...;

static inline glm::vec2 coordConvert2d(glm::vec2 coord, glm::vec2 trans, float r) {
    glm::vec2 coord_converted = glm::vec2(0.0f);

    coord_converted.x = cos(r) * (coord.x - trans.x) + sin(r) * (coord.y - trans.y);
    coord_converted.y = cos(r) * (coord.y - trans.y) - sin(r) * (coord.x - trans.x);

    return coord_converted;
}

static inline unsigned char insideOthoArea(glm::vec2 *convertedCoord, float *areaWH, float margin) {

    unsigned char result = 0;
    if (convertedCoord && areaWH) {
        float x = convertedCoord->x, y = convertedCoord->y;
        float w = areaWH[0], h = areaWH[1];

        if ( x >= 0 - margin && x <= w + margin && 
             y >= 0 - margin && y <= h + margin ) {
            result++;
        }
    }

    return result;
}

unsigned char insideArea(glm::vec3 *pos, rect *area, float margin, glm::vec2 *converted) {
    unsigned char result = 0;

    // Convert original coord (x,y) into (x', y') according to bottomRect's (area) leftDown-vertex-coord-system
    glm::vec2 pos2d = glm::vec2(pos->x, pos->y);
    glm::vec2 trans = area->leftDown; 
    float r = area->rotZ;
    glm::vec2 converted_pos2d = coordConvert2d(pos2d, trans, r);

    if (converted) {
        *converted = converted_pos2d;
    }

    // Assign w/h accoridng to bottomRect(area)
    float othoArea[] = { area->width, area->height };
    if (insideOthoArea(&converted_pos2d, othoArea, margin)) {
        result++;
    }

    return result;
}

rect::rect(glm::vec3 ctr, glm::vec3 ld, float *size_rot) {

    glm::vec3 givenPointPosition = glm::vec3(0.0f, 0.0f, 0.0f);  // If center position is provided, leftDown will be calculated, and center-position is supposed tobe at Zero before trans/rot, vice versa.
    glm::vec3 orgTargetPosition = glm::vec3(0.0f, 0.0f, 0.0f);   // Position of the vertex tobe calculated before trans/rot
    glm::vec3 *ptrTarget = NULL;                                 // points to the vertex tobe calculated (center or leftBofftom)

    // BUG in this design: if center coor is 000, then the rect could not be inited with center coord,
    // it could only be inited with leftbottom coord
    // vice versa
    if ( ctr != glm::vec3(0.0f, 0.0f, 0.0f) && size_rot ) {

        // center position is provided, and is supposed tobe at 0,0,0 position 
        // before translation/rotation in this case
        center = givenPointPosition = ctr;

        // leftDown vertex's position shall be calculated according to center and rotZ in this case
        orgTargetPosition = glm::vec3(-(size_rot[0] / 2), -(size_rot[1] / 2), ctr.z);
        ptrTarget = &leftDown;
    }
    else if ( ld != glm::vec3(0.0f, 0.0f, 0.0f) && size_rot ) {

        // leftDown position is provided, and is supposed tobe at 0,0,0 position 
        // before translation/rotation in this case
        leftDown = givenPointPosition = ld;

        // center vertex's position shall be calculated according to leftDown and rotZ in this case
        orgTargetPosition = glm::vec3( (size_rot[0] / 2), (size_rot[1] / 2), ld.z);
        ptrTarget = &center;
    }
    else {
        // Both center and leftDown vertex poisition not provided,
        // unable to init. Or, size is NULL pointer.
        return;
    }

    // Init width, height, and rotation Angle.
    width = size_rot[0]; height = size_rot[1]; 
    rotZ = size_rot[2];

    // Init the position for the 'leftDown' or 'center' vertex, according to param input
    glm::mat4 transMat = glm::translate(glm::mat4(1.0f), glm::vec3(givenPointPosition.x, givenPointPosition.y, 0.0f));
    glm::mat4   rotMat = glm::rotate(glm::mat4(1.0f), size_rot[2], glm::vec3(0.0f, 0.0f, 1.0f));
    glm::mat4 modelMat = transMat * rotMat;
    *ptrTarget = glm::vec3(modelMat * glm::vec4(orgTargetPosition, 1.0));

    // Init the directions for the 4 normals
    // glm::vec3 normalBeforeRot[nIdx::max] = {
    //     glm::vec3( 0.0f,  1.0f, 0),  // north, Z should be 0 for 2d normal !!!
    //     glm::vec3( 1.0f,  0.0f, 0),  // east
    //     glm::vec3( 0.0f, -1.0f, 0),  // south
    //     glm::vec3(-1.0f,  0.0f, 0)   // west
    // };
    // for (unsigned char i = 0; i < nIdx::max; i++) {
    //     normal[i] = glm::vec3( rotMat * glm::vec4(normalBeforeRot[i], 0.0f) );
    // }

    // Init the collisionInfo
    collisionInfo = 0;
}

cube::cube(glm::vec3 rectCenter, glm::vec3 rectLeftDown, float *size_rot, float cubeHeight) {
    // : bottomRect(rectCenter, rectLeftDown, size_rot) {
    bottomRect = rect(rectCenter, rectLeftDown, size_rot);
    height = cubeHeight;

    glm::vec3 vtBeforeTrans[NUM_VERTICES_FOR_CUBE] = {
        glm::vec3(          0,           0, 0),               // vt0 
        glm::vec3(          0,           0, cubeHeight),  // vt1 

        glm::vec3(size_rot[0],           0, 0),               // 
        glm::vec3(size_rot[0],           0, cubeHeight),  // 

        glm::vec3(size_rot[0], size_rot[1], 0),               // 
        glm::vec3(size_rot[0], size_rot[1], cubeHeight),  // 

        glm::vec3(          0, size_rot[1], 0),               // 
        glm::vec3(          0, size_rot[1], cubeHeight)   // 
    };

    glm::mat4 transMat = glm::translate(glm::mat4(1.0f), bottomRect.leftDown);
    glm::mat4   rotMat = glm::rotate(glm::mat4(1.0f), size_rot[2], glm::vec3(0.0f, 0.0f, 1.0f));
    glm::mat4 modelMat = transMat * rotMat;
    
    for (unsigned char i = 0; i < NUM_VERTICES_FOR_CUBE; i++) {
        vertices[i] = glm::vec3(modelMat * glm::vec4(vtBeforeTrans[i], 1.0f));
    }
}

void triggerPad::init(triggerUpdater* p) {
    updater = p;
}

void triggerPad::update(float delta) {
    if ( updater ) {
        updater->update(this, delta);
    }
}



// Updaters for different triggers
void changeViewUpdater::update(triggerPad *trigger, float delta) {
    // printf("  update for ChangeView is called.\n\n");
}

void changePlaneUpdater::update(triggerPad *trigger, float delta) {

    static glm::vec3 vector = glm::vec3(0.0f);
    static float anim_time = 0, sum_time = 0;
    static float speed = 0;

    unsigned char status = (trigger) ? (trigger->status) : 0;
    // printf("  changePlaneUpdater::update() called\n");

    if (TRIGGER_STATUS_UPDATING & status) {

        if (sum_time < anim_time) {
            // calculate the player position's change value according to vector/speed/delta
            glm::vec3 vector_ = delta * speed * vector;
            // printf("  vector_: %f, %f, %f.\n", vector_.x, vector_.y, vector_.z);

            // Tobe removed, this shall be done in test layer, trigger update only deals with data,
            // then testlayer's update gets the data, then renders with this data.
            // (*trigger->pos) += vector_;
            trigger->enteringPosRot[0] = vector_.x;
            trigger->enteringPosRot[1] = vector_.y;
            trigger->enteringPosRot[2] = vector_.z;

            // printf("  ChangePlane() sum: %f, delta:%f.\n\n", sum_time, delta);

            status &= ~TRIGGER_STATUS_FIRST_UPDATE_DONE;
            sum_time += delta;
        }
        else if (sum_time >= anim_time) {
            printf("  ChangePlane() last call.\n\n");

            status &= ~TRIGGER_STATUS_FIRST_UPDATE_DONE;
            status &= ~TRIGGER_STATUS_UPDATING;
            status |= TRIGGER_STATUS_LAST_UPDATE_DONE;
        }
    }
    else {
        // status = (trigger) ? trigger->status : 0;

        // if ( (status & TRIGGER_STATUS_POS_VALID) &&
        //      (status & TRIGGER_STATUS_ANG_VALID) &&
        //      (status & TRIGGER_STATUS_INPUT_VALID) ) {

        //     anim_time = (trigger->anim_time) * (trigger->anim_repeat);
        //     sum_time = 0;
        //     vector = glm::normalize(trigger->vector);
        //     speed = trigger->speed;

        //     status |= TRIGGER_STATUS_UPDATING;
        // }

        anim_time = (trigger) ? (trigger->anim_time) * (trigger->anim_repeat) : 0.0f;
        sum_time = 0.0f;
        vector = (trigger) ? ( glm::vec3(trigger->movingVecSpd[0], trigger->movingVecSpd[1], trigger->movingVecSpd[2] ) ) : glm::vec3(0.0f);
        if (vector != glm::vec3(0.0f)) {
            vector = glm::normalize(vector);
        }
        speed = (trigger) ? trigger->movingVecSpd[3] : 0;

        status |= TRIGGER_STATUS_FIRST_UPDATE_DONE;
        status |= TRIGGER_STATUS_UPDATING;
        status &= ~TRIGGER_STATUS_LAST_UPDATE_DONE;
    }

    if (trigger) {
        trigger->status = status;
    }
}

void openDoorUpdater::update(triggerPad *trigger, float delta) {
    static float anim_time = 0, sum_time = 0;

    // printf("444444\n");
    unsigned char status = (trigger) ? (trigger->status) : 0;
    // printf("  changePlaneUpdater::update() called\n");

    if (TRIGGER_STATUS_UPDATING & status) {

        // printf("5555555\n");
        unsigned char user_input_to_skip_open_door_anim = 0;
        if (sum_time < anim_time) {
            // printf("  update() for open-door sum: %f, delta:%f.\n\n", sum_time, delta);

            // To play some animation ... 
            //... ...
            // (*trigger->pos).x = trigger->enteringPosRot[0];
            // (*trigger->pos).y = trigger->enteringPosRot[1];
            // (*trigger->pos).z = trigger->enteringPosRot[2];
            // (*trigger->rot) = trigger->enteringPosRot[3];
            // printf("  open-door-anim");
            printf("  d");

            status &= ~TRIGGER_STATUS_FIRST_UPDATE_DONE;
            sum_time += delta;
        }
        else if (sum_time >= anim_time || user_input_to_skip_open_door_anim) {
            printf("\n  open-door update() called for the last time.\n\n");

            status &= ~TRIGGER_STATUS_FIRST_UPDATE_DONE;
            status &= ~TRIGGER_STATUS_UPDATING;
            status |= TRIGGER_STATUS_LAST_UPDATE_DONE;
        }

            // (*trigger->pos).x = trigger->enteringPosRot[0];
            // (*trigger->pos).y = trigger->enteringPosRot[1];
            // (*trigger->pos).z = trigger->enteringPosRot[2];
            // (*trigger->rot) = trigger->enteringPosRot[3];
            // status &= ~TRIGGER_STATUS_UPDATING;
    }
    else {
        printf("  OpenDoor() updating will start in next frame.\n\n");
        anim_time = (trigger) ? (trigger->anim_time) * (trigger->anim_repeat) : 0.0f;
        sum_time = 0.0f;

        status |= TRIGGER_STATUS_FIRST_UPDATE_DONE;
        status |= TRIGGER_STATUS_UPDATING;
        status &= ~TRIGGER_STATUS_LAST_UPDATE_DONE;
    }

    if (trigger) {
        trigger->status = status;
    }
}

// void ...Updater::update(triggerPad *trigger, float delta) { 
//     ... 
// }
