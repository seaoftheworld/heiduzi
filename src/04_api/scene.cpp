#include "scene.h"
#include "scene_data.h"



// The '_VALUE' value depends on the framerate/delta-time.
// When it is 0.03', it works fine if Intel HD 3000 GPU is used, 
// running at dynamic frame rate from 200 ~ 300/400 fps. 
//
// But if Nvd gpu is used, the frame rate is fixed at 60 frames/second,
// thus the delta is larger, making one step in a frame too big,
// the 0.03 value causes software to calculate player's position 
// tobe inside obsCube, causing trouble.
//
// It's better to make delta fixed in the future, then this value could be fixed
    // The '0.03' value here is better to be calculated according to 'delta' or fps.
    // Too big (0.1 for example) would cause the obsCube to return wrong normal,
    // (on obsCube1 and 8's T-connection for example, cube8 shall return west, but north returned).
    // Too small (0.01) would make the player pass through the wall.
    // How to calculate acurately ???
    // Only 'des' is used now, 'from' shall also be used ???
#define _VALUE (delta * 2.5 * 1.5)  // 2.5 is the forward speed of player, '_VALUE' is 1.5 times of 1 step

// #define MARGIN_VALUE (delta * 2.5 * 4) // delta * 2.5 / 10 ==> forward speed for player is 2.5
#define MARGIN_VALUE (0.05f)  // has tobe fixed, otherwise there's bugs





// can be replaced by static global var
struct hdzScene {
    static scenePlane plane[MAX_NUM_PLANE];
    static unsigned char numPlanes;
    static unsigned char plIdx;

    static inline scenePlane *getCurrentPlane();
    static inline void setCurrentPlane(unsigned char i);
};

scenePlane hdzScene::plane[] = {};
unsigned char hdzScene::numPlanes = 0;
unsigned char hdzScene::plIdx = 0;

scenePlane *hdzScene::getCurrentPlane() {
    if (plIdx >= numPlanes)
        return NULL;

    return &plane[plIdx];
}

void hdzScene::setCurrentPlane(unsigned char i) {
    if (i >= numPlanes)
        i = numPlanes;

    plIdx = i;
}



static unsigned char numNearObsCube = 0;
static unsigned char nearObsCubeIdx[MAX_NUM_OBSTACLE_CUBE] = {  };

// Shall be only 1 valid trigger each frame ???
static unsigned char numValidTrgPad = 0;
static unsigned char validTrgPadIdx[MAX_NUM_TRIGGERS] = {  };

static unsigned char sceneStatus = 0;

// room info from Open-Door trigger
static targetPlaceInfo targetPlace = {};

// position/plane-idx info from change-plane trigger
static targetPlaneInfo targetPositionPlane = {};

// These data should be read in from file latter, now they are in array
sampleInitData initData;
samplePorchData porchData;
sampleYardData  yardData;









// Convert the 2d-coord (x, y) in original coordinate system into
// (x', y') in translated and rotated 2d coordinate system
static glm::vec2 coordConvert2d(glm::vec2 coord, glm::vec2 trans, float r) {
    glm::vec2 coord_converted = glm::vec2(0.0f);

    coord_converted.x = cos(r) * (coord.x - trans.x) + sin(r) * (coord.y - trans.y);
    coord_converted.y = cos(r) * (coord.y - trans.y) - sin(r) * (coord.x - trans.x);

    return coord_converted;
}

unsigned char scene_getStatus(unsigned char bitMask) {
    return (sceneStatus & bitMask);
}
unsigned char scene_getStatus() {
    return sceneStatus;
}
void scene_setStatus(unsigned char bitMask, unsigned char set) {
    if (set) {
        sceneStatus |= bitMask;
    }
    else {
        sceneStatus &= ~bitMask;
    }
}
void scene_setStatus(unsigned char sts) {
    sceneStatus = sts;
}

static inline void scene_setTargetPlace(unsigned char room, unsigned char plane, float *pos_rot) {
    switch (room) {
    case thePorch:
        printf("  porch from open-door trigger.\n");
        targetPlace.rm = thePorch;
        break;

    case theYard:
        printf("  yard from open-door trigger.\n");
        targetPlace.rm = theYard;
        break;

    case roomMax:
    default:
        printf("  roomMax ??? from open-door trigger.\n");
        targetPlace.rm = roomMax;
        break;
    }

    targetPlace.pl = plane;

    if (pos_rot) {
        targetPlace.enteringPosRot[0] = pos_rot[0];
        targetPlace.enteringPosRot[1] = pos_rot[1];
        targetPlace.enteringPosRot[2] = pos_rot[2];
        targetPlace.enteringPosRot[3] = pos_rot[3];
    }
}

targetPlaceInfo *scene_getTargetPlace() {
    return &targetPlace;
}

static inline void scene_setTargetPositionPlane(float *pos) {
    if (pos) {
        targetPositionPlane.pos[0] = pos[0];
        targetPositionPlane.pos[1] = pos[1];
        targetPositionPlane.pos[2] = pos[2];
    }
}

targetPlaneInfo *scene_getTargetPositionPlane() {
    return &targetPositionPlane;
}

scenePlane *scene_getCurrentPlane() {
    return hdzScene::getCurrentPlane();
}

void scene_setCurrentPlane(unsigned char i) {
    hdzScene::setCurrentPlane(i);
    sceneStatus &= ~SCENE_STATUS_CHANGE_PLANE_INDEX;
}

void scene_LoadData(room r, unsigned char targetPlane) {

    planeData** ppPlaneData = NULL;
    switch (r) {
    // case roomInit:
    //     ppPlaneData = initData.plane;
    //     break;
        
    case thePorch:
        ppPlaneData = porchData.plane;
        break;

    case theYard:
        ppPlaneData = yardData.plane;
        break;

    case roomMax:
    default:
        break;
    }
    

    // Iterate through the planes contained in the scene data and initialized the 'plane[]' array accordingly
    unsigned char planeIdx = 0;
    hdzScene::numPlanes = 0;
    while( *ppPlaneData != NULL ) {
        // printf("  numPlane: %d.\n", numPlanes);
        scenePlane *plane = &hdzScene::plane[planeIdx];

        // Iterate through the obsAreas contained in the plane data and initialized the 'obsCube[]' array accordingly
            unsigned char obsCubeIdx = 0;
            plane->numObsCubes = 0;

            areaData *aData = (*ppPlaneData)->obsAreas;
            if (!aData) { return; }

            while (aData->w != 0 && aData->h != 0) {

                float size_rot[] = {aData->w, aData->h, aData->rotZ};
                plane->obsCube[obsCubeIdx] = cube(aData->center, aData->leftDown, size_rot, ZHEIGHT_OBSTACLE_CUBE);

                obsCubeIdx++;
                aData++;
            }
            plane->numObsCubes = obsCubeIdx;

        // Iterate through the other items contained in the plane data and initialized the other arraies accordingly
        //... ...
            unsigned char triggerIdx = 0;
            plane->numTriggers = 0;

            triggerData *tData = (*ppPlaneData)->trgAreas;
            if (!tData) { return; }

            while (tData->type != NULL) {

                // Assign value for plane.trigger.pad and setting values for general values
                areaData *aData = &tData->area; float size_rot[] = {aData->w, aData->h, aData->rotZ};
                plane->trigger[triggerIdx].pad =
                    cube(aData->center, aData->leftDown, size_rot, ZHEIGHT_TRIGGER_PAD);

                float angle = tData->info.valid_angle;
                if (angle > __PI) {  
                    do { angle -= __2_PI; } while (angle > __PI);
                }
                else if (angle <= -__PI) {
                    do { angle += __2_PI; } while (angle <= -__PI);
                }

                plane->trigger[triggerIdx].valid_angle = angle;
                plane->trigger[triggerIdx].anim_time = tData->info.anim_time;
                plane->trigger[triggerIdx].anim_repeat = tData->info.anim_repeat;
                plane->trigger[triggerIdx].status = 0;

                // process according to plane.trigger.type
                //... ...
                const char *type = tData->type;
                if (type == TRIGGER_TYPE_CHANGE_PLANE) {
                    plane->trigger[triggerIdx].type = trigger_ChangePlane;
                    plane->trigger[triggerIdx].init(&updater_ChangePlane);

                    plane->trigger[triggerIdx].movingVecSpd[0] = tData->info.vec_spd.x;
                    plane->trigger[triggerIdx].movingVecSpd[1] = tData->info.vec_spd.y;
                    plane->trigger[triggerIdx].movingVecSpd[2] = tData->info.vec_spd.z;
                    plane->trigger[triggerIdx].movingVecSpd[3] = tData->info.vec_spd.s;

                    plane->trigger[triggerIdx].targetPlaneIdx = tData->info.targetPlaneIdx;

                    printf("change-plane trigger inited, target plane: %d\n", plane->trigger[triggerIdx].targetPlaneIdx);
                    // printf("change-plane trigger status: 0x%2x\n", plane->trigger[triggerIdx].status);
                }
                else if (type == TRIGGER_TYPE_CHANGE_VIEW) {
                    plane->trigger[triggerIdx].type = trigger_ChangeView;
                    plane->trigger[triggerIdx].init(&updater_ChangeView);

                    //... ...
                }
                else if (type == TRIGGER_TYPE_OPEN_DOOR) {
                    plane->trigger[triggerIdx].type = trigger_OpenDoor;
                    plane->trigger[triggerIdx].init(&updater_OpenDoor);

                    plane->trigger[triggerIdx].enteringPosRot[0] = tData->info.pos_rot.x;
                    plane->trigger[triggerIdx].enteringPosRot[1] = tData->info.pos_rot.y;
                    plane->trigger[triggerIdx].enteringPosRot[2] = tData->info.pos_rot.z;
                    plane->trigger[triggerIdx].enteringPosRot[3] = tData->info.pos_rot.r;

                    plane->trigger[triggerIdx].targetRoomIdx = tData->info.targetRoomIdx;
                    plane->trigger[triggerIdx].targetPlaneIdx = tData->info.targetPlaneIdx;

                    printf("open-door trigger inited, target room/plane: %d/%d\n", 
                        plane->trigger[triggerIdx].targetRoomIdx,
                        plane->trigger[triggerIdx].targetPlaneIdx);
                    // printf("open-door trigger status: 0x%2x\n", plane->trigger[triggerIdx].status);
                }
                else {
                }

                triggerIdx++;
                tData++;
            }
            plane->numTriggers = triggerIdx;

        // Iterate through the other items contained in the plane data and initialized the other arraies accordingly
        //... ...
            // unsigned char liftIdx = 0;
            // plane->numLifts = 0;

            // transferData *lData = (*ppPlaneData)->trfAreas;
            // if (!lData) { return; }

            // while (lData->speed != 0) {

            //     // Assign value for plane.lift.pad
            //     areaData *aData = &lData->area; float size_rot[] = {aData->w, aData->h, aData->rotZ};
            //     plane->lift[liftIdx].pad =
            //         cube(aData->center, aData->leftDown, size_rot, ZHEIGHT_LIFT_PAD);

            //         plane->lift[liftIdx].transAnimTtime = lData->transAnimTtime;
            //         plane->lift[liftIdx].transAnimRepeat = lData->transAnimRepeat;
            //         plane->lift[liftIdx].transVector.x = lData->x;
            //         plane->lift[liftIdx].transVector.y = lData->y;
            //         plane->lift[liftIdx].transVector.z = lData->z;
            //         plane->lift[liftIdx].transSpeed = lData->speed;
            //         plane->lift[liftIdx].targetPlaneIdx = lData->targetPlaneIdx;

            //         // Make the angle within: [0, PI] (-0, -PI) range, consistent with player's ang range
            //         float angle = lData->angle;
            //         while (angle > __PI) {  
            //             angle -= __2_PI;
            //         }
            //         while (angle <= -__PI) {
            //             angle += __2_PI;
            //         }
            //         plane->lift[liftIdx].validAngle = angle;

            //         printf("liftData inited, to plane: %d\n", plane->lift[liftIdx].targetPlaneIdx);

            //     liftIdx++;
            //     lData++;
            // }
            // plane->numLifts = liftIdx;

        planeIdx++;
        ppPlaneData++;
    }
    hdzScene::numPlanes = planeIdx;

    // Iterate through the other items contained in the scene data and initialized the other arraies accordingly
    //... ...

    // Set the hdzScene::plIdx
    if (hdzScene::numPlanes == 0) {
        // at least 1 plane should exist, error
        printf("\n\n");
        printf("  No plane is found in this scene data !\n");
        printf("  At least 1 plane shall exist !%d\n", hdzScene::plIdx);
        printf("\n\n");
        return;
    }

    if (targetPlane >= hdzScene::numPlanes) {
        hdzScene::plIdx = hdzScene::numPlanes - 1;
    }
    else {
        hdzScene::plIdx = targetPlane;
    }

    printf("\n\n");
    printf("  scene data loaded\n");
    printf("    numPlanes: %d\n", hdzScene::numPlanes);
    for (unsigned char i = 0; i < hdzScene::numPlanes; i++) {
        printf("      plane[%d] has \'%d\' obsAreas\n", i, hdzScene::plane[i].numObsCubes);
        printf("                    \'%d\' triggers\n", hdzScene::plane[i].numTriggers);
        // printf("                    \'%d\' lifs\n", hdzScene::plane[i].numLifts);
    }
    printf("    now on plane %d\n", hdzScene::plIdx);
    printf("\n\n");
}


static unsigned char scene_findValidTriggerPads(glm::vec3 *pos, float *rot, unsigned char input) {
    numValidTrgPad = 0;
    scenePlane *plane = hdzScene::getCurrentPlane();

    // printf("222222\n");

    for (unsigned char i = 0; i < plane->numTriggers; i++) {

        // Verify is the position passed in inside triggerPad's bottom rectangle
        rect *trgPadBotRect = &plane->trigger[i].pad.bottomRect;

        if ( TRIGGER_STATUS_UPDATING & plane->trigger[i].status ) {
            validTrgPadIdx[numValidTrgPad] = i;  // Valid since it's updating
            numValidTrgPad++;

            // Only 1 trigger triggered every time ???
            // printf(" find updating trigger idx: %d\n\n", validTrgPadIdx[numValidTrgPad]);
            break;
        }
        else {
            // Quit if Input invalid
            // if ( (BITMASK_FIRE & input) && !(input & BITMASK_READY) ) {
            if ( !(BITMASK_FIRE & input) || (BITMASK_READY & input) ) {
                continue;
            }

            // Quit if Angle invalid
            float rotTrigger = plane->trigger[i].valid_angle;

            if (*rot - rotTrigger > __3_PI_4) {
                rotTrigger += __2_PI;
            }
            else if (*rot - rotTrigger < -__3_PI_4) {
                rotTrigger -= __2_PI;
            }
            float r = *rot - rotTrigger;

            if (r >= __PI_4 || r <= -__PI_4) {
                continue;
            }

            // Quit if pos invalid
            if ( !insideArea(pos, trgPadBotRect, 0, NULL) ) {
                continue;  
            }

            // Mark this trigger if input/angle/position are all valid && it is not being updated
            validTrgPadIdx[numValidTrgPad] = i;  // Valid since pos/ang/input all valid && not updating
            numValidTrgPad++;


            // The pos/rot pointed by input parameter might be changed, eg,
            // changePlane-trigger's update has to change player's pos (and also rot in the near future)
            // openDoor-trigger's update has to set player's new pos/rot in the new room
            plane->trigger[i].pos = pos;  
            plane->trigger[i].rot = rot;                    

            // open-door status-bit cleared after vbo and texture are allocated
            // sceneStatus |= (plane->trigger[i].type == trigger_OpenDoor) ? (SCENE_STATUS_PLAYING_OPEN_DOOR_ANIMATION) : (0);
            // if (plane->trigger[i].type == trigger_OpenDoor) {
            //     sceneStatus |= SCENE_STATUS_PLAYING_OPEN_DOOR_ANIMATION;
            //     sceneStatus |= SCENE_STATUS_ALLOCATING_VBO_AND_TEXTURE;
            // }
            // if (sceneStatus & SCENE_STATUS_PLAYING_OPEN_DOOR_ANIMATION) {
            //     printf("  Openning-Door() status bit set.\n\n");
            // }

            printf(" find valid trigger idx: %d\n", validTrgPadIdx[numValidTrgPad]);
            printf("      valid trigger num: %d\n\n", numValidTrgPad);

            // Only 1 trigger triggered every time ???
            break;
        }
    }


    // if (num) {
    //     printf("    num   :%d\n\n", num);
    // }
    return numValidTrgPad;
}

static unsigned char scene_updateValidTriggerPads(float delta) {
    // unsigned char num = 0;
    scenePlane *plane = hdzScene::getCurrentPlane();

    // Should only update 1 trigger each time ???
    // for (unsigned char i = 0; i < numValidTrgPad; i++) {
    //     triggerPad *trigger = &plane->trigger[0] + validTrgPadIdx[i];
        triggerPad *trigger = &plane->trigger[0] + validTrgPadIdx[0];

        trigger->update(delta);

        // Change the plane-index after change-plane-trigger has finished updating
        if ( trigger->type == trigger_ChangePlane) { 
            if ( trigger->status & TRIGGER_STATUS_FIRST_UPDATE_DONE ) {
                targetPositionPlane.plIdx = trigger->targetPlaneIdx;
            }
            else if ( trigger->status & TRIGGER_STATUS_UPDATING ) {
                scene_setTargetPositionPlane(trigger->enteringPosRot);

                sceneStatus |= SCENE_STATUS_UPDATE_PLAYER_BY_CHANGE_PLANE;
            }
            else if ( trigger->status & TRIGGER_STATUS_LAST_UPDATE_DONE ) {
                printf("  ChangePlane() updating ended.\n\n");
                
                sceneStatus &= ~SCENE_STATUS_UPDATE_PLAYER_BY_CHANGE_PLANE;
                sceneStatus |= SCENE_STATUS_CHANGE_PLANE_INDEX;
            }
        }
        
        if ( trigger->type == trigger_OpenDoor) { 
            if ( trigger->status & TRIGGER_STATUS_FIRST_UPDATE_DONE ) {
                // Get data for the room to be entered from door-trigger
                scene_setTargetPlace(trigger->targetRoomIdx, trigger->targetPlaneIdx, trigger->enteringPosRot);

                sceneStatus |= SCENE_STATUS_PLAYING_OPEN_DOOR_ANIMATION;
                sceneStatus |= SCENE_STATUS_ALLOCATING_VBO_AND_TEXTURE;  // tobe cleared in testLayer
            }
            else if ( trigger->status & TRIGGER_STATUS_UPDATING ) {
            }
            else if ( trigger->status & TRIGGER_STATUS_LAST_UPDATE_DONE ) {
                printf("  OpenDoor() updating ended.\n\n");

                sceneStatus &= ~SCENE_STATUS_PLAYING_OPEN_DOOR_ANIMATION;
            }
        }

        if (trigger->status & TRIGGER_STATUS_UPDATING) {
            return 1;
        }
        else {
            return 0;
        }
    // }

    // if (num) {
    //     printf("    num   :%d\n\n", num);
    // }
    // return num;
}

void scene_updateTriggerAndStatus(float delta, glm::vec3 *pos, float *rot, unsigned char input) {
    if ( (SCENE_STATUS_PLAYING_OPEN_DOOR_ANIMATION & sceneStatus) || 
         (SCENE_STATUS_ALLOCATING_VBO_AND_TEXTURE & sceneStatus) ) {

        // Do not quit this branch until both playing open-door animation is finished,
        // and allocating-buffer is finished.
        // Open-door animation bit is cleared when open-door's last update() is called,
        // Allocating buffer bit is cleared in test layer after buffer is allocated.
        if (SCENE_STATUS_PLAYING_OPEN_DOOR_ANIMATION & sceneStatus) {
            scene_updateValidTriggerPads(delta);  // open-door trigger's update () is expected to be called here
        }
    }
    else {
        if (scene_findValidTriggerPads(pos, rot, input)) {
            scene_updateValidTriggerPads(delta);

            sceneStatus &= ~SCENE_STATUS_UPDATE_PLAYER_BY_INPUT;
        }
        else {
            sceneStatus |= SCENE_STATUS_UPDATE_PLAYER_BY_INPUT;
        }
    }
}



unsigned char scene_NearObsCube(float delta, glm::vec3 desPos) {  // delta is used for calculating '_VALUE'
    numNearObsCube = 0;

    scenePlane *plane = hdzScene::getCurrentPlane();
    for (unsigned char i = 0; i < plane->numObsCubes; i++) {

        // Verify is destination position inside obsCube's bottom rectangle + margin
        rect *obsCubeBotRect = &plane->obsCube[i].bottomRect;

        glm::vec2 coord_converted = glm::vec2(0.0f);
        if ( insideArea(&desPos, obsCubeBotRect, MARGIN_VALUE, &coord_converted) ) {

                // Optional: process with combo normal if the position is inside left-top/down, right-top/down margins of the obsCube.
                // Otherwise, when 2 obsCube connects, there might be bugs for this algrithom,
                // wrong normal might be returned.
                //
                //         + == |---------| == + 
                //         [skip)         (skip] MARGIN_VALUE
                //     --- + == +--- w ---+ == + ---
                //         |    |         |    |
                //         |    h obsCube h    |
                //         |    |         |    |
                //     --- + == 0--- w ---+ == + ---
                //         [skip)         (skip] MARGIN_VALUE
                //         + == +---------| == +      
                //                        |MARG|
                //
                // Without these: player may stuck at connection of 2 obs-cubes (only when facing almost exactly to the connection)\
                // But with these: Only when facing a conner at 45 degree, pressing forward doesnt makes the position change anymore when close enough,
                //                 when not exactly 45 degrees, moving towrds the obs-conner, player will get too closer to obs,
                //                 thus, its found the player will stuck to conner when moving to the other direction after too close (b2 branch),
                //                 unless moving towards a direction quite opposite from the conner.
                // #define X_IN_LEFT_MARGIN  (coord_converted.x < 0 && coord_converted.x >= -MARGIN_VALUE)
                // #define X_IN_RIGHT_MARGIN (coord_converted.x > obsWidth && coord_converted.x <= obsWidth + MARGIN_VALUE)
                
                // #define Y_IN_UP_MARGIN    (coord_converted.y > obsHeight && coord_converted.y <= obsHeight + MARGIN_VALUE)
                // #define Y_IN_DOWN_MARGIN  (coord_converted.y < 0 && coord_converted.y >= -MARGIN_VALUE)

                // if ( (X_IN_LEFT_MARGIN || X_IN_RIGHT_MARGIN) && (Y_IN_UP_MARGIN || Y_IN_DOWN_MARGIN) ) {
                //     result = 1;
                //     nearObsCubeIdx[numNearObsCube] = i;
                //     numNearObsCube++;

                //     printf("  __ on obsCube: %d\n", i);
                //     // printf("      delta: %f\n", delta);
                //     // printf("     Margin: %f\n", MARGIN_VALUE);
                //     // printf("     obs wh_rot: %f, %f, %f\n", obsWidth, obsHeight, obsRot);
                //     // printf("     obs Left Down: %f, %f\n", obsLeftDownXY.x, obsLeftDownXY.y);
                //     // printf("     obs Center   : %f, %f\n", obsCubeBotRect->center.x, obsCubeBotRect->center.y);
                //     // printf("     obs Rect col : %2x", obsCubeBotRect->collisionInfo);
                //     printf("converted_coord: %f, %f\n\n", coord_converted.x, coord_converted.y);

                //     // if ( (obsCubeBotRect->collisionInfo & COLLISION_INFO_BIT_MASK_COLLISION_VALID) == 0 ) {
                //     //     obsCubeBotRect->collisionInfo = (COLLISION_INFO_BIT_MASK_COLLISION_VALID | 0);

                //         if (X_IN_LEFT_MARGIN && Y_IN_DOWN_MARGIN) {
                //             obsCubeBotRect->collisionInfo |= (1 << 4);  // 1000 0000, south-west
                //         }

                //         if (X_IN_RIGHT_MARGIN && Y_IN_DOWN_MARGIN) {
                //             obsCubeBotRect->collisionInfo |= (2 << 4);  // 1001 0000, south-east
                //         }

                //         if (X_IN_RIGHT_MARGIN && Y_IN_UP_MARGIN) {
                //             obsCubeBotRect->collisionInfo |= (3 << 4);  // 1010 0000, north-east
                //         }

                //         if (X_IN_LEFT_MARGIN && Y_IN_UP_MARGIN) {       // 1011 0000, north-west
                //             obsCubeBotRect->collisionInfo |= (4 << 4);
                //         }
                //     // }
                //     continue;
                // }


            nearObsCubeIdx[numNearObsCube] = i;
            numNearObsCube++;

            printf("  __ near obsCube-%d\n", i);
            printf("      delta: %f\n", delta);
            // printf("     Margin: %f\n", MARGIN_VALUE);
            // printf("     obs wh_rot: %f, %f, %f\n", obsWidth, obsHeight, obsRot);
            // printf("     obs Left Down: %f, %f\n", obsLeftDownXY.x, obsLeftDownXY.y);
            // printf("     obs Center   : %f, %f\n", obsCubeBotRect->center.x, obsCubeBotRect->center.y);
            // printf("     obs Rect col : %2x", obsCubeBotRect->collisionInfo);
            printf("converted_coord: %f, %f\n", coord_converted.x, coord_converted.y);

            // printf("  __ col info 0: 0x%2x\n", obsMesh[i].collisionlInfo);
            if ( (obsCubeBotRect->collisionInfo & COLLISION_INFO_BIT_MASK_COLLISION_VALID) == 0 ) {

                // obsMesh[i].collisionlInfo |= COLLISION_INFO_BIT_MASK_COLLISION_VALID;       // This line is Wrong !!!
                // obsMesh[i].collisionlInfo = (COLLISION_INFO_BIT_MASK_COLLISION_VALID | 0);  // shall clear all the other bits and only set 1 bit
                obsCubeBotRect->collisionInfo = (COLLISION_INFO_BIT_MASK_COLLISION_VALID | 0);

                // printf("  __ col info 1: 0x%2x\n", obsMesh[i].collisionlInfo);
                float _value = (float)_VALUE;
                //
                // if (Y_IN_UP_MARGIN) {  // when the margin-value is 0.05, it's too thin that once the code get here, player already passed through the up-margin
                if (obsCubeBotRect->height + MARGIN_VALUE - coord_converted.y < _value) {
                    obsCubeBotRect->collisionInfo |= COLLISION_INFO_BIT_MASK_NORMAL_NORTH;  // only 1-bit for valid and another 1-bit is set
                }
                // if (X_IN_RIGHT_MARGIN) {
                else if (obsCubeBotRect->width + MARGIN_VALUE - coord_converted.x < _value) {
                    obsCubeBotRect->collisionInfo |= COLLISION_INFO_BIT_MASK_NORMAL_EAST;
                }
                // if (Y_IN_DOWN_MARGIN) {
                else if (coord_converted.y - (-MARGIN_VALUE) < _value) {
                    obsCubeBotRect->collisionInfo |= COLLISION_INFO_BIT_MASK_NORMAL_SOUTH;
                }
                // if (X_IN_LEFT_MARGIN) {
                else if (coord_converted.x - (-MARGIN_VALUE) < _value) {
                    obsCubeBotRect->collisionInfo |= COLLISION_INFO_BIT_MASK_NORMAL_WEST;
                }
                else {
                    // This shall not be reached, it would cause trouble (player doesnt move) 
                    // When fps is lower --> delta longer --> step larger, this may be reached.
                    // If that happens, the '0.03' value shall be increased accordingly.
                    obsCubeBotRect->collisionInfo |= COLLISION_INFO_BIT_MASK_INSIDE_RECT;
                }
            }
            // printf("  __ col info 2: 0x%2x\n\n", obsMesh[i].collisionlInfo);
        }
        else {
            // printf("  __ col info 3: 0x%2x\n\n", obsMesh[i].collisionlInfo);
            
            // obsCubeBotRect->collisionInfo = 0;
            obsCubeBotRect->collisionInfo &= ~COLLISION_INFO_BIT_MASK_COLLISION_VALID;

            // printf("  __ outide mesh[%d]\n", i);
            // printf("  __ col info: 0x%2x\n\n", obsMesh[i].collisionlInfo);
        }

        if (i == plane->numObsCubes - 1 && numNearObsCube) {
            printf("\n");
        }
    }

    return numNearObsCube;
}

glm::vec3 scene_NormalFromNearObsCube() {
    glm::vec3 hitNormal = glm::vec3(0.0f);

    scenePlane *plane = hdzScene::getCurrentPlane();
    // for (unsigned char i = 0; i < plane->numObsCubes; i++) {
    for (unsigned char i = 0; i < numNearObsCube; i++) {

        // rect *obsCubeBotRect = &plane->obsCube[i].bottomRect;
        // rect *obsCubeBotRect = &plane->obsCube[0].bottomRect + nearObsCubeIdx[i];
        rect *obsCubeBotRect = &(&plane->obsCube[0] + nearObsCubeIdx[i])->bottomRect;

        if (obsCubeBotRect->collisionInfo & COLLISION_INFO_BIT_MASK_COLLISION_VALID) {

            // printf("  __ normal from obsCube: %d\n", i);
            printf("     col info: %2x\n", obsCubeBotRect->collisionInfo);
            
            switch (obsCubeBotRect->collisionInfo & ~COLLISION_INFO_BIT_MASK_COLLISION_VALID) {
            // switch (obsCubeBotRect->collisionInfo & ~0xf0) {

            case COLLISION_INFO_BIT_MASK_NORMAL_NORTH:
                hitNormal += obsCubeBotRect->normal[nIdx::north];
                break;

            case COLLISION_INFO_BIT_MASK_NORMAL_EAST:
                hitNormal += obsCubeBotRect->normal[nIdx::east];
                break;

            case COLLISION_INFO_BIT_MASK_NORMAL_SOUTH:
                hitNormal += obsCubeBotRect->normal[nIdx::south];
                break;

            case COLLISION_INFO_BIT_MASK_NORMAL_WEST:
                hitNormal += obsCubeBotRect->normal[nIdx::west];
                break;

            // 4 special areas
            // case (COLLISION_INFO_BIT_MASK_NORMAL_SOUTH | COLLISION_INFO_BIT_MASK_NORMAL_WEST):
            //     hitNormal += glm::normalize(obsCubeBotRect->normal[nIdx::south] + obsCubeBotRect->normal[nIdx::west]);
            //     break;

            // case (COLLISION_INFO_BIT_MASK_NORMAL_SOUTH | COLLISION_INFO_BIT_MASK_NORMAL_EAST):
            //     hitNormal += glm::normalize(obsCubeBotRect->normal[nIdx::south] + obsCubeBotRect->normal[nIdx::east]);
            //     break;

            // case (COLLISION_INFO_BIT_MASK_NORMAL_NORTH | COLLISION_INFO_BIT_MASK_NORMAL_EAST):
            //     hitNormal += glm::normalize(obsCubeBotRect->normal[nIdx::north] + obsCubeBotRect->normal[nIdx::east]);
            //     break;

            // case (COLLISION_INFO_BIT_MASK_NORMAL_NORTH | COLLISION_INFO_BIT_MASK_NORMAL_WEST):
            //     hitNormal += glm::normalize(obsCubeBotRect->normal[nIdx::north] + obsCubeBotRect->normal[nIdx::west]);
            //     break;


            case COLLISION_INFO_BIT_MASK_INSIDE_RECT:
                default:
                //... ...
                break;                
            }

        }

        if (i == plane->numObsCubes - 1) {
            printf("\n");
        }

    }

    // Normalize the normal-vector
    //... ...
    if (hitNormal != glm::vec3(0.0f)) {
        hitNormal = glm::normalize(hitNormal);
    }

    // if (hitNormal != glm::vec3(0.0f)) {
    //     return glm::normalize(hitNormal);
    // }
    // else
    //     return hitNormal;

    // return glm::normalize(hitNormal);

    return hitNormal;
}

unsigned char scene_InNearObsCube(glm::vec3 desPos) {
    unsigned char num = 0;
    scenePlane *plane = hdzScene::getCurrentPlane();

    // for (unsigned char i = 0; i < plane->numObsCubes; i++) {
    for (unsigned char i = 0; i < numNearObsCube; i++) {

        // Verify is destination position inside obsCube's bottom rectangle
        rect *obsCubeBotRect = &(&plane->obsCube[0] + nearObsCubeIdx[i])->bottomRect;
        
        if ( insideArea(&desPos, obsCubeBotRect, MARGIN_VALUE, NULL) ) {
            num++;
            // printf("    i     :%d\n", i);
            // printf("    in obs:%d\n", nearObsCubeIdx[i]);
        }
    }

    // if (num) {
    //     printf("    num   :%d\n\n", num);
    // }
    return num;
}
