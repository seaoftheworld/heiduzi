#include "scene.h"

#include "test_data_000.h"  // contains collision/trigger/cam data of the bedroom 
#include "test_data_001.h"  // contains collision/trigger/cam data for the 'backYard'



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
static unsigned char nearObsCubeIdx[MAX_NUM_OBSTACLE_CUBE] = {};

// Shall be only 1 valid trigger each frame ???
static unsigned char numValidTrgPad = 0;
static unsigned char validTrgPadIdx[MAX_NUM_TRIGGERS] = {};

// static unsigned char numCameras = 0;
static unsigned char currentCamIdx = 0;
static unsigned char numCurrentCamSwitch = 0;

static unsigned char sceneStatus = 0;

// room info from Open-Door trigger
static targetPlaceInfo targetPlace = {};

// position/plane-idx info from change-plane trigger
static targetPlaneInfo targetPositionPlane = {};

static const glm::vec3 normal[nIdx::max] = {
    glm::vec3( 0.0f,  1.0f, 0.0f),
    glm::vec3( 1.0f,  0.0f, 0.0f),
    glm::vec3( 0.0f, -1.0f, 0.0f),
    glm::vec3(-1.0f,  0.0f, 0.0f)
};

// These data should be read in from file latter, now they are in array
#ifdef __TEST00__
    test00_Data backyardData;
#elif defined __TEST01__
    test01_Data backyardData;
#else
    SampleBackyardData backyardData;
#endif

sampleStreetData  streetData;









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

// static inline void scene_setTargetPlace(unsigned char room, unsigned char plane, float *pos_rot) {
static inline void scene_setTargetPlace(unsigned char *room_plane_camera, float *pos_rot) {
    if (room_plane_camera) {
        switch ( room_plane_camera[0] ) {
        case backYard:
            printf("  \'backYard\' from open-door trigger.\n");
            targetPlace.rm = backYard;
            break;

        case theStreet:
            printf("  \'theStreet\' from open-door trigger.\n");
            targetPlace.rm = theStreet;
            break;

        case roomMax:
        default:
            printf("  roomMax ??? from open-door trigger.\n");
            targetPlace.rm = roomMax;
            break;
        }

        targetPlace.plane = room_plane_camera[1];
        targetPlace.camIdx = room_plane_camera[2];
    }

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

static void setCollisionStatusBit(glm::vec2 *pos, rect *rec, float margin, float refVal) {

    rec->collisionInfo &= ~COLLISION_BIT_MASK_CLEAR_STATUS;

    if (rec->height + MARGIN_VALUE - pos->y < refVal) {
        rec->collisionInfo |= COLLISION_STATUS_NORMAL_NORTH;  // only 1-bit for valid and another 1-bit is set
    }
    else if (rec->width + MARGIN_VALUE - pos->x < refVal) {
        rec->collisionInfo |= COLLISION_STATUS_NORMAL_EAST;
    }
    else if (pos->y - (-MARGIN_VALUE) < refVal) {
        rec->collisionInfo |= COLLISION_STATUS_NORMAL_SOUTH;
    }
    else if (pos->x - (-MARGIN_VALUE) < refVal) {
        rec->collisionInfo |= COLLISION_STATUS_NORMAL_WEST;
    }
    else {
        // This shall not be reached, it would cause trouble (player doesnt move) 
        // When fps is lower --> delta longer --> step larger, this may be reached.
        // If that happens, the '0.03' value shall be increased accordingly.
        rec->collisionInfo |= COLLISION_BIT_MASK_INSIDE_RECT;
    }
}

static glm::mat4 rotMatFromRect(rect *rec) {
    if (!rec) {
        return glm::mat4(1.0f);
    }

    return glm::rotate(glm::mat4(1.0f), rec->rotZ, glm::vec3(0.0f, 0.0f, 1.0f));
}

static inline glm::vec3 normalFromCollision(rect *rec) {
    if (!rec) {
        return glm::vec3(0.0f);
    }

    switch (rec->collisionInfo & ~COLLISION_BIT_MASK_VALID) {

        case COLLISION_STATUS_NORMAL_NORTH:
            return (rec->rotZ == 0.0f) ? (normal[nIdx::north]) : 
                glm::vec3( rotMatFromRect(rec) * glm::vec4(normal[nIdx::north], 0.0f) );
            break;

        case COLLISION_STATUS_NORMAL_EAST:
            return (rec->rotZ == 0.0f) ? (normal[nIdx::east]) : 
                glm::vec3( rotMatFromRect(rec) * glm::vec4(normal[nIdx::east], 0.0f) );
            break;

        case COLLISION_STATUS_NORMAL_SOUTH:
            return (rec->rotZ == 0.0f) ? (normal[nIdx::south]) : 
                glm::vec3( rotMatFromRect(rec) * glm::vec4(normal[nIdx::south], 0.0f) );
            break;

        case COLLISION_STATUS_NORMAL_WEST:
            return (rec->rotZ == 0.0f) ? (normal[nIdx::west]) : 
                glm::vec3( rotMatFromRect(rec) * glm::vec4(normal[nIdx::west], 0.0f) );
            break;

        case COLLISION_BIT_MASK_INSIDE_RECT:
        default:
            //... ...
            return glm::vec3(0.0f);
            break;                
    }
}

#define X_IN_RIGHT_MARGIN(pos, rect, margin) (pos.x > rect->width  && pos.x <= rect->width + margin)
#define X_IN_LEFT_MARGIN(pos, rect, margin)  (pos.x < 0            && pos.x >= (0 - margin))
#define Y_IN_UP_MARGIN(pos, rect, margin)    (pos.y > rect->height && pos.y <= rect->height + margin)
#define Y_IN_DOWN_MARGIN(pos, rect, margin)  (pos.y < 0            && pos.y >= (0 - margin))

#define ON_CONNER_AREAS(pos, rect, margin) ( \
 (X_IN_LEFT_MARGIN(pos, rect, margin) || X_IN_RIGHT_MARGIN(pos, rect, margin)) &&\
 (Y_IN_UP_MARGIN(pos, rect, margin) || Y_IN_DOWN_MARGIN(pos, rect, margin)) )

#define VALID_COLLISION(col) (col & COLLISION_BIT_MASK_VALID)





scenePlane *scene_getCurrentPlane() {
    return hdzScene::getCurrentPlane();
}

void scene_setCurrentPlane(unsigned char i) {
    hdzScene::setCurrentPlane(i);
    sceneStatus &= ~SCENE_STATUS_CHANGE_PLANE_INDEX;
}

void scene_LoadData(room r, unsigned char targetPlane, unsigned char targetCamIdx) {

    planeData** ppPlaneData = NULL;
    switch (r) {
    // case roomInit:
    //     ppPlaneData = initData.plane;
    //     break;
        
    case backYard:
        // ppPlaneData = porchData.plane;
        ppPlaneData = backyardData.plane;
        break;

    case theStreet:
        ppPlaneData = streetData.plane;
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
            while (aData && aData->w != 0 && aData->h != 0) {

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
            while (tData && tData->type != NULL) {

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
                    plane->trigger[triggerIdx].targetCameraIdx = tData->info.targetCameraIdx;

                    printf("open-door trigger inited, target room/plane/cam: %d/%d/%d\n", 
                        plane->trigger[triggerIdx].targetRoomIdx,
                        plane->trigger[triggerIdx].targetPlaneIdx,
                        plane->trigger[triggerIdx].targetCameraIdx);
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

            unsigned char camIdx = 0;
            unsigned char allCamSwIdx = 0;
            plane->numCams = 0;

            cameraData **cData = (*ppPlaneData)->cam;
            while (cData && *cData) {
                // Assign value for plane->cam[]/numCams
                    plane->cam[camIdx].index = camIdx;
                    plane->cam[camIdx].position.x = (*cData)->pos_rot.x;
                    plane->cam[camIdx].position.y = (*cData)->pos_rot.y;
                    plane->cam[camIdx].position.z = (*cData)->pos_rot.z;
                    plane->cam[camIdx].hAngle = (*cData)->pos_rot.hRot;
                    plane->cam[camIdx].vAngle = (*cData)->pos_rot.vRot;

                // Assign value for plane->allCamSw[]/numAllCamSw
                    camSwitchData *camSw = (*cData)->sw;
                    unsigned char numSw = 0;

                    if (camSw->area.w == 0.0f && camSw->area.h == 0.0f) {
                        plane->cam[camIdx].sw = NULL;
                    }
                    else {
                        plane->cam[camIdx].sw = &plane->allCamSw[allCamSwIdx];

                        while ( camSw->area.w != 0.0f && camSw->area.h != 0.0f ) {
                            areaData *area = &camSw->area; 
                            float size_rot[] = {area->w, area->h, area->rotZ};

                            // printf("allCamSwIdx: %d, wh: %f, %f\n", allCamSwIdx, camSw->area.w, camSw->area.h);
                            // plane->allCamSw[allCamSwIdx].area = rect(area->center, area->leftDown, size_rot);
                            plane->allCamSw[allCamSwIdx].pad = cube(area->center, area->leftDown, size_rot, ZHEIGHT_CAM_SW_PAD);
                            plane->allCamSw[allCamSwIdx].targetCamIdx = camSw->targetIndex;

                            allCamSwIdx++;
                            numSw++;
                            camSw++;
                        }
                    }
                    plane->cam[camIdx].numSw = numSw;

                    printf("\ncam-%d data inited, it has: %d sw, allCamSw num is \'%d\' now\n\n", camIdx, numSw, allCamSwIdx);

                camIdx++;
                cData++;
            }
            plane->numCams = camIdx;
            plane->numAllCamSw = allCamSwIdx;

            // index of camera shall be independent from plane in the future
            if (plane->numCams != 0) {
                currentCamIdx = (targetCamIdx >= plane->numCams) ? (plane->numCams - 1) : targetCamIdx;
            }
            else {
                // provide a default camera if no camData is found from the file/data
                plane->cam[0].position = glm::vec3(1.0f);
                plane->cam[0].hAngle = 0.0f;
                plane->cam[0].vAngle = 0.0f;
                plane->cam[0].index = 0;
                plane->cam[0].sw = NULL;
                
                plane->numCams = 1;
                currentCamIdx = 0;
            }



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

    hdzScene::plIdx = (targetPlane >= hdzScene::numPlanes) ? (hdzScene::numPlanes - 1) : (targetPlane);
    // currentCamIdx = (targetCamIdx >= plane->numCams) ? (plane->numCams - 1) : targetCamIdx;

    printf("\n\n");
    printf("  scene data loaded\n");
    printf("    numPlanes: %d\n", hdzScene::numPlanes);
    for (unsigned char i = 0; i < hdzScene::numPlanes; i++) {
        printf("      plane[%d] has \'%d\' obsAreas\n", i, hdzScene::plane[i].numObsCubes);
        printf("                    \'%d\' triggers\n", hdzScene::plane[i].numTriggers);
        // printf("                    \'%d\' lifs\n", hdzScene::plane[i].numLifts);
        printf("                    \'%d\' cams\n", hdzScene::plane[i].numCams);
    }
    printf("    now on plane %d\n", hdzScene::plIdx);
    printf("    now at cam %d\n", currentCamIdx);
    printf("\n\n");
}


unsigned char scene_updateCurrentCameraIndex(glm::vec3 playerPos) {
    scenePlane *plane = hdzScene::getCurrentPlane();

    for (unsigned char i = 0; i < plane->cam[currentCamIdx].numSw; i++) {
        if ( insideArea(&playerPos, &plane->cam[currentCamIdx].sw[i].pad.bottomRect, 0, NULL) ) {
            currentCamIdx = plane->cam[currentCamIdx].sw[i].targetCamIdx;
            break;
        }
    }

    return currentCamIdx;
}

unsigned char scene_getCurrentCameraIndex() {
    return currentCamIdx;
}

void scene_getCamPosRot(unsigned char camIdx, glm::vec3 *pos, float *rot) {
    scenePlane *plane = hdzScene::getCurrentPlane();

    if (camIdx < plane->numCams) {

        if (pos) {
            *pos = plane->cam[camIdx].position;
        }

        if (rot) {
            rot[0] = plane->cam[camIdx].hAngle;
            rot[1] = plane->cam[camIdx].vAngle;
        }
    }

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
                unsigned char target_room_plane_camera[] = {
                    trigger->targetRoomIdx, trigger->targetPlaneIdx, trigger->targetCameraIdx
                };
                scene_setTargetPlace(target_room_plane_camera, trigger->enteringPosRot);

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

        return (trigger->status & TRIGGER_STATUS_UPDATING) ? 1 : 0;
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



unsigned char scene_findCollidingObsCube(float delta, glm::vec3 desPos) {  // delta is used for calculating '_VALUE'

    float _value = (float)_VALUE;
    unsigned char review = 0;

    numNearObsCube = 0;

    // When the other obs-cube has already had a valid collision, skip a obs-cube if play's position 
    // is inside conner area: left-top/down && right-top/down areas of the obsCube;
    // Otherwise, when the player moves to where 2 obsCubes connect, the unexpected normal might be returned,
    // making the player stuck at the intersection of 2 obs-cubes:
    //
    //           |  |
    //        ---+--+ <--(may stuck here)
    //              |
    //        ------+ 
    //
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
    scenePlane *plane = hdzScene::getCurrentPlane();
    for (unsigned char i = 0; i < plane->numObsCubes; i++) {

        rect *obsCubeBotRect = &plane->obsCube[i].bottomRect;
        glm::vec2 coord_converted = glm::vec2(0.0f);

        // Verify is destination position inside obsCube's bottom rectangle + margin
        if ( insideArea(&desPos, obsCubeBotRect, MARGIN_VALUE, &coord_converted) ) {

            printf("  __ near obsCube-%d\n", i);
            printf("      delta: %f\n", delta);
            // printf("     Margin: %f\n", MARGIN_VALUE);
            // printf("     obs wh_rot: %f, %f, %f\n", obsWidth, obsHeight, obsRot);
            // printf("     obs Left Down: %f, %f\n", obsLeftDownXY.x, obsLeftDownXY.y);
            // printf("     obs Center   : %f, %f\n", obsCubeBotRect->center.x, obsCubeBotRect->center.y);
            // printf("     obs Rect col : %2x", obsCubeBotRect->collisionInfo);
            printf("des-coord converted: %f, %f\n", coord_converted.x, coord_converted.y);

            if ( ON_CONNER_AREAS(coord_converted, obsCubeBotRect, MARGIN_VALUE) ) {
                if ( VALID_COLLISION(obsCubeBotRect->collisionInfo) ) {
                    // Collision has been valid before the player's postition reaches the obs-cube's cornner,
                    // which means the player reached the conner by moving along 1 side of the rect, 
                    // remain the collision status of the obs-cube in this situation.
                }
                else {
                    // The player's postition reached the obs-cube's cornner-area first before reaching the other areas, 
                    // this kind of collision will be valid only when no other obs-cubes are collided after iteration.
                    // If the other obs are also found to have been collided during review, this conner collision is ommited.
                    setCollisionStatusBit(&coord_converted, obsCubeBotRect, MARGIN_VALUE, _value);

                    review = i;
                    continue;
                }
            }
            else {
                // No longer needed with conner design
                // Only updates the collision info after player is away from the obs-cube (collision-valid bit is 0)
                // Otherwise remain the previous collision info even if reached the intersection.
                // if ( !VALID_COLLISION(obsCubeBotRect->collisionInfo) ) {
                    setCollisionStatusBit(&coord_converted, obsCubeBotRect, MARGIN_VALUE, _value);
                    obsCubeBotRect->collisionInfo |= COLLISION_BIT_MASK_VALID;
                // }
            }

            nearObsCubeIdx[numNearObsCube] = i;
            numNearObsCube++;
        }
        else {
            // printf("  __ outide mesh[%d]\n", i);
            // printf("  __ col info: 0x%2x\n\n", obsMesh[i].collisionInfo);
            obsCubeBotRect->collisionInfo &= ~COLLISION_BIT_MASK_VALID;
            obsCubeBotRect->collisionInfo &= ~COLLISION_BIT_MASK_INSIDE_RECT;
        }
    }

    if (review) {
        rect *obsCubeBotRect = &plane->obsCube[review].bottomRect;
        
        if (numNearObsCube == 0) {
            obsCubeBotRect->collisionInfo |= COLLISION_BIT_MASK_VALID;

            nearObsCubeIdx[numNearObsCube] = review;
            numNearObsCube++;
        }
    }

    if (numNearObsCube) {
        printf("\n");
    }

    return numNearObsCube;
}

#define __10_MARGIN (MARGIN_VALUE + MARGIN_VALUE + \
    MARGIN_VALUE + MARGIN_VALUE + MARGIN_VALUE + MARGIN_VALUE + \
    MARGIN_VALUE + MARGIN_VALUE + MARGIN_VALUE + MARGIN_VALUE)
unsigned char scene_markActiveObsCube(float delta, glm::vec3 desPos) {

    // numActiveObsCube = 0;

    scenePlane *plane = hdzScene::getCurrentPlane();
    for (unsigned char i = 0; i < plane->numObsCubes; i++) {

        rect *obsCubeBotRect = &plane->obsCube[i].bottomRect;

        // Verify is destination position inside obsCube's bottom rectangle + 10 * margin,
        // if inside, this obs-cube should be marked as near.
        if ( insideArea(&desPos, obsCubeBotRect, __10_MARGIN, NULL) ) {
            // set active bit, or update the active-obs-cubes' index
            //... ...
            // activeObsCubeIdx[numActiveObsCube] = i;
            // numActiveObsCube++;

            // call something like the previous function
            //... ...
        }
        else {
            // clear active bit
            //... ...
        }
    }

    // return numActiveObsCube;
    return 0;
}

glm::vec3 scene_normalFromCollidingObsCube() {
    glm::vec3 hitNormal = glm::vec3(0.0f);
    scenePlane *plane = hdzScene::getCurrentPlane();

    for (unsigned char i = 0; i < numNearObsCube; i++) {

        rect *obsCubeBotRect = &(&plane->obsCube[0] + nearObsCubeIdx[i])->bottomRect;

        if ( (obsCubeBotRect->collisionInfo & COLLISION_BIT_MASK_VALID) ) {
            // printf("  __ normal from obsCube: %d\n", i);
            printf("     col info: 0x%2x\n", obsCubeBotRect->collisionInfo);
            hitNormal += normalFromCollision(obsCubeBotRect);
        }

        if (i == plane->numObsCubes - 1) {
            printf("\n");
        }
    }

    return ( hitNormal == glm::vec3(0.0f) ) ? hitNormal : glm::normalize(hitNormal);
}

unsigned char scene_insideCollidingObsCube(glm::vec3 desPos) {
    unsigned char num = 0;
    scenePlane *plane = hdzScene::getCurrentPlane();

    // for (unsigned char i = 0; i < plane->numObsCubes; i++) {
    for (unsigned char i = 0; i < numNearObsCube; i++) {

        // Verify is destination position inside obsCube's bottom rectangle
        rect *obsCubeBotRect = &(&plane->obsCube[0] + nearObsCubeIdx[i])->bottomRect;
        
        glm::vec2 coord_converted(0.0f);
        if ( insideArea(&desPos, obsCubeBotRect, MARGIN_VALUE, &coord_converted) ) {
            // printf("    i     :%d\n", i);
            // printf("    in obs:%d\n", nearObsCubeIdx[i]);
            printf("  final des Inside near-obs: %f, %f\n\n", coord_converted.x, coord_converted.y);
            num++;
        }
        else {
            printf("  final des Outside near-obs: %f, %f\n\n", coord_converted.x, coord_converted.y);
        }
    }
    return num;
}

unsigned char scene_insideAnyObsCube(glm::vec3 desPos) {
    unsigned char num = 0;
    scenePlane *plane = hdzScene::getCurrentPlane();

    for (unsigned char i = 0; i < plane->numObsCubes; i++) {
    // for (unsigned char i = 0; i < numNearObsCube; i++) {

        // Verify is destination position inside obsCube's bottom rectangle
        // rect *obsCubeBotRect = &(&plane->obsCube[0] + nearObsCubeIdx[i])->bottomRect;
        rect *obsCubeBotRect = &plane->obsCube[i].bottomRect;
        
        glm::vec2 coord_converted(0.0f);
        if ( insideArea(&desPos, obsCubeBotRect, MARGIN_VALUE, &coord_converted) ) {
            printf("  \n\nfinal des Inside obs-%d: %f, %f\n", i, coord_converted.x, coord_converted.y);
            num++;
        }
        else {
            // printf("  final des Outside near-obs: %f, %f\n\n", coord_converted.x, coord_converted.y);
        }
    }
    return num;
}
