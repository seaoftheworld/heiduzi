#include "scene.h"
#include "scene_data.h"



// The '_VALUE' value depends on the framerate/delta-time.
// When it's 0.03', it works fine if Intel HD 3000 GPU is used, 
// running at dynamic frame rate from 200 ~ 300/400 fps. 
//
// But when Nvd gpu is used, the frame rate is fixed at 60 frames/second,
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
#define _VALUE (delta * 2.5 * 1.5)  // 2.5 is the forward speed of player

// #define MARGIN_VALUE (delta * 2.5 * 4) // delta * 2.5 / 10 ==> forward speed for player is 2.5
#define MARGIN_VALUE (0.05f)  // has tobe fixed, otherwise there's bugs

static unsigned char numTouchedObsCube = 0;
static unsigned char touchedObsCubeIdx[MAX_NUM_OBSTACLE_CUBE] = {  };



// can be replaced by static global var
struct hdzScene {
    static scenePlane plane[MAX_NUM_PLANE];
    static unsigned short numPlanes;
    static unsigned short plIdx;

    static inline scenePlane *getCurrentPlane();
    static inline void setCurrentPlane(unsigned short i);
};

scenePlane hdzScene::plane[] = {};
unsigned short hdzScene::numPlanes = 0;
unsigned short hdzScene::plIdx = 0;

scenePlane *hdzScene::getCurrentPlane() {
    if (plIdx >= numPlanes)
        return NULL;

    return &plane[plIdx];
}

void hdzScene::setCurrentPlane(unsigned short i) {
    if (i >= numPlanes)
        i = numPlanes;

    plIdx = i;
}



// Convert the 2d-coord (x, y) in original coordinate system into
// (x', y') in translated and rotated 2d coordinate system
static glm::vec2 coordConvert2d(glm::vec2 coord, glm::vec2 trans, float r) {
    glm::vec2 coord_converted = glm::vec2(0.0f);

    coord_converted.x = cos(r) * (coord.x - trans.x) + sin(r) * (coord.y - trans.y);
    coord_converted.y = cos(r) * (coord.y - trans.y) - sin(r) * (coord.x - trans.x);

    return coord_converted;
}

scenePlane *scene_GetCurrentPlane() {
    return hdzScene::getCurrentPlane();
}

void scene_SetCurrentPlane(unsigned char i) {
    hdzScene::setCurrentPlane(i);
}

void scene_LoadData(room r) {
    sampleSceneData sampleSceneData;
    planeData **ppPlaneData = sampleSceneData.plane;

    // Iterate through the planes contained in the scene data and initialized the 'plane[]' array accordingly
    unsigned char planeIdx = 0;
    hdzScene::numPlanes = 0;
    while( *ppPlaneData != NULL ) {
        // printf("  numPlane: %d.\n", numPlanes);
        scenePlane *plane = &hdzScene::plane[planeIdx];

        // Iterate through the obsCubes contained in the plane data and initialized the 'obsCube[]' array accordingly
            unsigned char obsCubeIdx = 0;
            plane->numObsCubes = 0;

            cubeData *cData = (*ppPlaneData)->obsCubes;
            while (cData->w != 0 && cData->h != 0) {

                float size_rot[] = {cData->w, cData->h, cData->rotZ};
                plane->obsCube[obsCubeIdx] = cube(cData->center, cData->leftDown, size_rot, ZHEIGHT_OBSTACLE_CUBE);

                obsCubeIdx++;
                cData++;
            }
            plane->numObsCubes = obsCubeIdx;

        // Iterate through the other items contained in the plane data and initialized the other arraies accordingly
        //... ...
            unsigned char triggerIdx = 0;
            plane->numTriggers = 0;

            triggerPadData *tData = (*ppPlaneData)->trgPads;
            while (tData->type != NULL) {

                // assign value for plane.trigger.pad
                cubeData *cData = &tData->pad; float size_rot[] = {cData->w, cData->h, cData->rotZ};
                plane->trigger[triggerIdx].pad =
                    cube(cData->center, cData->leftDown, size_rot, ZHEIGHT_TRIGGER_PAD);

                // process according to plane.trigger.type
                //... ...
                // auto pf = std::bind(updateChangePlane, std::placeholders::_1, std::placeholders::_2);
                // plane->trigger[triggerIdx].update = pf;
                // (&plane->trigger[0] + triggerIdx)->init(&updater_ChangePlane);
                plane->trigger[triggerIdx].init(&updater_ChangePlane);

                triggerIdx++;
                tData++;
            }
            plane->numTriggers = triggerIdx;

        planeIdx++;
        ppPlaneData++;
    }
    hdzScene::numPlanes = planeIdx;

    // Iterate through the other items contained in the scene data and initialized the other arraies accordingly
    //... ...

    printf("\n\n");
    printf("  scene data loaded\n");
    printf("    numPlanes: %d\n", hdzScene::numPlanes);
    for (unsigned char i = 0; i < hdzScene::numPlanes; i++) {
        printf("      plane[%d] has \'%d\' obsCubes\n", i, hdzScene::plane[i].numObsCubes);
        printf("                and \'%d\' triggers\n", hdzScene::plane[i].numTriggers);
    }
    printf("\n\n");
}

unsigned char scene_InObsCube(float delta, glm::vec3 destinationPosition) {
    unsigned char num = 0;
    scenePlane *plane = hdzScene::getCurrentPlane();

    for (unsigned char i = 0; i < numTouchedObsCube; i++) {

        // Verify is destination position inside obsCube's bottom rectangle
        // rect *obsCubeBotRect = &plane->obsCube[0].bottomRect + touchedObsCubeIdx[i];
        rect *obsCubeBotRect = &(&plane->obsCube[0] + touchedObsCubeIdx[i])->bottomRect;

        float obsWidth  = obsCubeBotRect->width;
        float obsHeight = obsCubeBotRect->height;
        
        // Convert the des.xyz into xyz in the mesh-rectangle's coordinate
        glm::vec2 desXY           = glm::vec2(destinationPosition.x, destinationPosition.y);
        glm::vec2 obsLeftDownXY = glm::vec2(obsCubeBotRect->leftDown.x, obsCubeBotRect->leftDown.y);
        float obsRot              = obsCubeBotRect->rotZ;

        glm::vec2 coord_converted = coordConvert2d(desXY, obsLeftDownXY, obsRot);

        // if (coord_converted.x >= 0 && coord_converted.x <=  obsWidth && 
        //      coord_converted.y >= 0 && coord_converted.y <=  obsHeight) {

        // des-point Still will enter the rect a little, so margin is required, otherwise, there's bug 
        // when 2 obs intersects. For eg, after reached wall corner, player will be stuck to the wall
        // (player was made entered cube0 a little more after reaching cube0/2's intersection).
        if ( coord_converted.x >= 0 - (MARGIN_VALUE) && coord_converted.x <=  obsWidth + (MARGIN_VALUE) && 
             coord_converted.y >= 0 - (MARGIN_VALUE) && coord_converted.y <=  obsHeight + (MARGIN_VALUE) ) {
            // return 1;
            num++;
            printf("    i     :%d\n", i);
            printf("    in obs:%d\n", touchedObsCubeIdx[i]);
        }
        else {
            // return 0;
        }
    }

    if (num) {
        printf("    num   :%d\n\n", num);
    }
    return num;
}

unsigned char scene_NearObsCube(float delta, glm::vec3 destinationPosition) {
    unsigned char result = 0;
    numTouchedObsCube = 0;

    scenePlane *plane = hdzScene::getCurrentPlane();
    for (unsigned char i = 0; i < plane->numObsCubes; i++) {

        // Verify is destination position inside obsCube's bottom rectangle + margin
        rect *obsCubeBotRect = &plane->obsCube[i].bottomRect;
        float obsWidth  = obsCubeBotRect->width;
        float obsHeight = obsCubeBotRect->height;

        // Convert the des.xyz into xyz in the mesh-rectangle's coordinate
        glm::vec2 desXY           = glm::vec2(destinationPosition.x, destinationPosition.y);
        glm::vec2 obsLeftDownXY = glm::vec2(obsCubeBotRect->leftDown.x, obsCubeBotRect->leftDown.y);
        float obsRot              = obsCubeBotRect->rotZ;

        glm::vec2 coord_converted = coordConvert2d(desXY, obsLeftDownXY, obsRot);
        
        
        if ( coord_converted.x >= 0 - (MARGIN_VALUE) && coord_converted.x <=  obsWidth + (MARGIN_VALUE) && 
             coord_converted.y >= 0 - (MARGIN_VALUE) && coord_converted.y <=  obsHeight + (MARGIN_VALUE) ) {
        // if ( coord_converted.x >= 0 && coord_converted.x <= obsWidth  && 
        //      coord_converted.y >= 0 && coord_converted.y <= obsHeight ) {

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
                //     touchedObsCubeIdx[numTouchedObsCube] = i;
                //     numTouchedObsCube++;

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

            result = 1;
            // result++;

            touchedObsCubeIdx[numTouchedObsCube] = i;
            numTouchedObsCube++;

            printf("  __ touched obsCube: %d\n", i);
            // printf("      delta: %f\n", delta);
            printf("     Margin: %f\n", MARGIN_VALUE);
            // printf("     obs wh_rot: %f, %f, %f\n", obsWidth, obsHeight, obsRot);
            // printf("     obs Left Down: %f, %f\n", obsLeftDownXY.x, obsLeftDownXY.y);
            // printf("     obs Center   : %f, %f\n", obsCubeBotRect->center.x, obsCubeBotRect->center.y);
            // printf("     obs Rect col : %2x", obsCubeBotRect->collisionInfo);
            printf("converted_coord: %f, %f\n", coord_converted.x, coord_converted.y);

            // printf("  __ col info 0: 0x%2x\n", obsMesh[i].collisionlInfo);
            if ( (obsCubeBotRect->collisionInfo & COLLISION_INFO_BIT_MASK_COLLISION_VALID) == 0 ) {

                // obsMesh[i].collisionlInfo |= COLLISION_INFO_BIT_MASK_COLLISION_VALID;       // This line is Wrong !!!
                // obsMesh[i].collisionlInfo = (COLLISION_INFO_BIT_MASK_COLLISION_VALID | 0);  // clear all the other bits and only set 1 bit
                obsCubeBotRect->collisionInfo = (COLLISION_INFO_BIT_MASK_COLLISION_VALID | 0);

                // printf("  __ col info 1: 0x%2x\n", obsMesh[i].collisionlInfo);
                float _value = _VALUE;
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

                    // float distance[] = {
                    //     obsHeight + MARGIN_VALUE - coord_converted.y,
                    //     obsWidth + MARGIN_VALUE - coord_converted.x,
                    //     coord_converted.y - (-MARGIN_VALUE),
                    //     coord_converted.x - (-MARGIN_VALUE)
                    // }
                    // // get the min distance into distance[0]
                    // for (unsigned short i = 0; (i + 1) < 4; i++) {
                    //     if (distance[i + 1] < distance[i]) {
                    //         float temp = distance[i];

                    //         distance[i] = distance[i + 1];
                    //         distance[i + 1] = temp;
                    //     }
                    // }
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

        if (i == plane->numObsCubes - 1 && result ) {
            printf("\n");
        }
    }

    // return result;
    return numTouchedObsCube;
}

glm::vec3 scene_NormalFromObsCube() {
    glm::vec3 hitNormal = glm::vec3(0.0f);

    scenePlane *plane = hdzScene::getCurrentPlane();
    // for (unsigned char i = 0; i < plane->numObsCubes; i++) {
    for (unsigned char i = 0; i < numTouchedObsCube; i++) {

        // rect *obsCubeBotRect = &plane->obsCube[i].bottomRect;
        // rect *obsCubeBotRect = &plane->obsCube[0].bottomRect + touchedObsCubeIdx[i];
        rect *obsCubeBotRect = &(&plane->obsCube[0] + touchedObsCubeIdx[i])->bottomRect;

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

/*
glm::vec3 sceneComboNormalFromObsCube() {

    glm::vec3 hitNormal = glm::vec3(0.0f);
    scenePlane *plane = hdzScene::getCurrentPlane();

    for (unsigned char i = 0; i < numTouchedObsCube; i++) {

        rect *obsCubeBotRect = &(&plane->obsCube[0] + touchedObsCubeIdx[i])->bottomRect;

        // if (obsCubeBotRect->collisionInfo & COLLISION_INFO_BIT_MASK_COLLISION_VALID) {

            // printf("  __ normal from obsCube: %d\n", i);
            // printf("     col info: %2x\n", obsCubeBotRect->collisionInfo);
            
            // switch (obsCubeBotRect->collisionInfo & ~COLLISION_INFO_BIT_MASK_COLLISION_VALID) {
            switch (obsCubeBotRect->collisionInfo) {

            // 4 special areas
            case (1 << 4):
                hitNormal += (obsCubeBotRect->normal[nIdx::south] + obsCubeBotRect->normal[nIdx::west]);
                break;

            case (2 << 4):
                hitNormal += (obsCubeBotRect->normal[nIdx::south] + obsCubeBotRect->normal[nIdx::east]);
                break;

            case (3 << 4):
                hitNormal += (obsCubeBotRect->normal[nIdx::north] + obsCubeBotRect->normal[nIdx::east]);
                break;

            case (4 << 4):
                hitNormal += (obsCubeBotRect->normal[nIdx::north] + obsCubeBotRect->normal[nIdx::west]);
                break;

            // case COLLISION_INFO_BIT_MASK_INSIDE_RECT:
                default:
                //... ...
                break;                
            }

        // }

        // if (i == plane->numObsCubes - 1) {
        //     printf("\n");
        // }
    }

    // Normalize the normal-vector
    //... ...
    if (hitNormal != glm::vec3(0.0f)) {
        // hitNormal = glm::normalize(hitNormal);
    }

    return hitNormal;
}
//*/

// void updateChangeView(triggerCube *trigger, float delta) {
//     // printf("  update for ChangeView is called.\n");
// }

// void updateChangePlane(triggerCube *trigger, float delta) {
//     // printf("  update for ChangePlane is called.\n");
// }
