#include "scene.h"

// These are supposed to be read from a file in the future
struct meshInfo {
    float w, h;
    glm::vec2 transXY;
    float rotZ;
};
meshInfo meshes_for_room00[] = {
    
    // {10,  1, glm::vec2(-2.0f, -2.0f), 0.0f },
    // { 1, 10, glm::vec2(-2.0f, -2.0f), 0.0f }, 
    // { 2,  2, glm::vec2( 2.0f,  2.0f), 3.14f / 6.0f },
    // { 0,  0, glm::vec2( 0.0f,  0.0f), 0.0f }

    {16.5f,  1.0f, glm::vec2(-7.5f,   2.5f), 0.0f },
    { 1.0f, 15.5f, glm::vec2(-7.5f, -12.5f), 0.0f }, 
    { 1.2f,  1.3f, glm::vec2( 1.4f,  -4.8f), -(3.14f / 6.0f) },
    { 7.8f,  2.8f, glm::vec2(-0.3f, -11.7f), 0.0f },
    {    0,     0, glm::vec2( 0.0f,  0.0f), 0.0f }
};


static const glm::vec3 unInitVertices[NUM_VERTICES_FOR_BOX] = {
    glm::vec3( 0.0f,  0.0f, 0.0f),
    glm::vec3( 0.0f,  0.0f, 1.0f),

    glm::vec3( 1.0f,  0.0f, 0.0f),
    glm::vec3( 1.0f,  0.0f, 1.0f),

    glm::vec3( 1.0f,  1.0f, 0.0f),
    glm::vec3( 1.0f,  1.0f, 1.0f),

    glm::vec3( 0.0f,  1.0f, 0.0f),
    glm::vec3( 0.0f,  1.0f, 1.0f)
};
static const glm::vec3 unInitNormal[obstacleMesh::normalId::max] = {
    glm::vec3( 0.0f,  1.0f, 0.0f),
    glm::vec3( 1.0f,  0.0f, 0.0f),
    glm::vec3( 0.0f, -1.0f, 0.0f),
    glm::vec3(-1.0f,  0.0f, 0.0f)
};

obstacleMesh::obstacleMesh(float *wh, glm::vec2 t, float r) {    
    for (unsigned char i = 0; i < NUM_VERTICES_FOR_BOX; i++) {
        vertices[i] = unInitVertices[i];
    }
    for (unsigned char i = north; i < max; i++) {
        normal[i] = unInitNormal[i];
    }
    xyWidth = 0; xyHeight = 0;

    transXY = t; rotZ = r;
    glm::mat4 transMat = glm::translate(glm::mat4(1.0f), glm::vec3(transXY, 0.0f));
    glm::mat4   rotMat = glm::rotate(glm::mat4(1.0f), rotZ, glm::vec3(0.0f, 0.0f, 1.0f));
    glm::mat4 modelMat = transMat * rotMat;

    if (wh) {
        xyWidth = wh[0]; xyHeight = wh[1];
        
        glm::vec3 initVertices[] = {
            glm::vec3(0.0f,     0.0f, 0.0f),
            glm::vec3(0.0f,     0.0f, ZHEIGHT),

            glm::vec3(xyWidth,     0.0f, 0.0f),
            glm::vec3(xyWidth,     0.0f, ZHEIGHT),

            glm::vec3(xyWidth, xyHeight, 0.0f),
            glm::vec3(xyWidth, xyHeight, ZHEIGHT),

            glm::vec3(0.0f, xyHeight, 0.0f),
            glm::vec3(0.0f, xyHeight, ZHEIGHT)
        };
        for (unsigned char i = 0; i < NUM_VERTICES_FOR_BOX; i++) {
            // glm::mat4 transMat = glm::translate(glm::mat4(1.0f), glm::vec3(transXY, 0.0f));
            // glm::mat4 rotMat = glm::rotate(glm::mat4(1.0f), rotZ, glm::vec3(0.0f, 0.0f, 1.0f));
            // glm::mat4 modelMat = transMat * rotMat;

            vertices[i] = glm::vec3( modelMat * glm::vec4(initVertices[i], 1.0f) );
        }
    }


    for (unsigned char i = north; i < max; i++) {
        // glm::mat4 transMat = glm::translate(glm::mat4(1.0f), glm::vec3(t, 0.0f));
        // glm::mat4 rotMat = glm::rotate(glm::mat4(1.0f), r, glm::vec3(0.0f, 0.0f, 1.0f) );
        // glm::vec3 normal_3 = glm::vec3( transMat * rotMat * glm::vec4(normal[i], 0.0f, 0.0f) );
        // normal[i] = glm::vec2(normal_3.x, normal_3.y);

        // glm::vec3 normal_3d = glm::vec3( modelMat * glm::vec4(normal[i], 0.0f, 0.0f) );
        // normal[i] = glm::vec2(normal_3d.x, normal_3d.y);

        normal[i] = glm::vec3( modelMat * glm::vec4(normal[i], 0.0f) );
    }
    collisionlInfo = 0;
}

void sceneSystem::loadSceneInfo() {

    // meshInfo* p = NULL;
    // p = meshes_for_room00;
    // numObstacleMesh = sizeof(*p) / sizeof(meshInfo);  // ???
    // printf("  __arraySize x:%d.\n", sizeof(*p));
    // printf("  __arraySize v:%d.\n", sizeof(meshes_for_room00));
    // printf("    element size:%d.\n", sizeof(meshInfo));
    // printf("    num Obstacle meshes loaded:%d.\n", numObstacleMesh);

    // for (unsigned char i = 0; i < numObstacleMesh && i < MAX_NUM_OBSTACLE_MESH; i++) {

    //     unsigned short wh[] = {
    //         meshes_for_room00[i].w,
    //         meshes_for_room00[i].h
    //     };

    //     obsMesh[i] = obstacleMesh(wh, meshes_for_room00[i].trans, meshes_for_room00[i].rotZ);
    // }

    meshInfo *p = NULL;
    numObstacleMesh = 0;

    p = meshes_for_room00;

    while ( p->w != 0 && p->h != 0 ) {
        float wh[] = {
            p->w,
            p->h
        };

        obsMesh[numObstacleMesh] = obstacleMesh(wh, p->transXY, p->rotZ);
        numObstacleMesh++;

        p++;
    }

    // for (unsigned char i = 0; i < MAX_NUM_OBSTACLE_MESH; i++) {
    //     hitObstacleMeshInfo[i] = obstacleMesh::normalId::max + 1;
    // }

    printf("  __col mesh num:%d\n", numObstacleMesh);
}

glm::vec2 sceneSystem::coordinateConvert(glm::vec2 coord, glm::vec2 trans, float r) {
    glm::vec2 coord_converted = glm::vec2(0.0f);

    coord_converted.x = cos(r) * (coord.x - trans.x) + sin(r) * (coord.y - trans.y);
    coord_converted.y = cos(r) * (coord.y - trans.y) - sin(r) * (coord.x - trans.x);

    return coord_converted;
}

glm::vec3 sceneSystem::normalFromObstacleRect() {
    glm::vec3 hitNormal = glm::vec3(0.0f, 0.0f, 0.0f);

    for (unsigned char i = 0; i < numObstacleMesh; i++) {

        if (obsMesh[i].collisionlInfo & COLLISION_INFO_BIT_MASK_COLLISION_VALID) {
            
            switch (obsMesh[i].collisionlInfo & ~COLLISION_INFO_BIT_MASK_COLLISION_VALID) {

            case COLLISION_INFO_BIT_MASK_NORMAL_NORTH:
                hitNormal += obsMesh[i].normal[obstacleMesh::normalId::north];
                break;

            case COLLISION_INFO_BIT_MASK_NORMAL_EAST:
                hitNormal += obsMesh[i].normal[obstacleMesh::normalId::east];
                break;

            case COLLISION_INFO_BIT_MASK_NORMAL_SOUTH:
                hitNormal += obsMesh[i].normal[obstacleMesh::normalId::south];
                break;

            case COLLISION_INFO_BIT_MASK_NORMAL_WEST:
                hitNormal += obsMesh[i].normal[obstacleMesh::normalId::west];
                break;

            case COLLISION_INFO_BIT_MASK_INSIDE_RECT:
                default:
                break;                
            }

            // Normalize the normal-vector
            //... ...
        }
    }

    return hitNormal;
}

unsigned char sceneSystem::onObstacleRect(glm::vec3 des) {
    unsigned char result = 0;
    // for (unsigned char i = 0; i < numObstacleMesh; i++) {
    //     hitObstacleMeshInfo[i] = obstacleMesh::normalId::max + 1;
    // }

    for (unsigned char i = 0; i < numObstacleMesh; i++) {
        // Convert the dex.xyz into xyz in the mesh-rectangle's coordinate
        glm::vec2 coord_converted = coordinateConvert(glm::vec2(des.x, des.y), obsMesh[i].transXY, obsMesh[i].rotZ);
        
        // If des` is inside mesh's rectangle
        if ( coord_converted.x >= 0 - (0.05f) && coord_converted.x <= obsMesh[i].xyWidth + (0.05f) && 
             coord_converted.y >= 0 - (0.05f) && coord_converted.y <= obsMesh[i].xyHeight + (0.05f) ) {

            // obsMeshIdx = i;
            // return 1;

            // hitObstacleMeshInfo[i] = 1;
            // result = 1;

            // if (obsMesh[i].xyHeight + (double)(0.05f) - coord_converted.y < 0.1) {
            //     hitObstacleMeshInfo[i] = obstacleMesh::normalId::north;
            // }
            // else if (obsMesh[i].xyWidth + (double)(0.05f) - coord_converted.x < 0.1) {
            //     // if () {
            //     hitObstacleMeshInfo[i] = obstacleMesh::normalId::east;
            //     // }
            // }
            // else if (coord_converted.y - (double)(-0.05f) < 0.1) {
            //     hitObstacleMeshInfo[i] = obstacleMesh::normalId::south;
            // }
            // else if (coord_converted.x - (double)(-0.05f) < 0.1) {
            //     hitObstacleMeshInfo[i] = obstacleMesh::normalId::west;
            // }
            // else {
            //     hitObstacleMeshInfo[i] = obstacleMesh::normalId::max;
            // }
            result = 1;
            // printf("  __ inside mesh %d\n", i);
            // printf("  __ col info 0: 0x%2x\n", obsMesh[i].collisionlInfo);

            if ((obsMesh[i].collisionlInfo & COLLISION_INFO_BIT_MASK_COLLISION_VALID) == 0) {

                // obsMesh[i].collisionlInfo |= COLLISION_INFO_BIT_MASK_COLLISION_VALID;
                obsMesh[i].collisionlInfo = (COLLISION_INFO_BIT_MASK_COLLISION_VALID | 0);

                // printf("  __ col info 1: 0x%2x\n", obsMesh[i].collisionlInfo);

                if (obsMesh[i].xyHeight + (double)(0.05f) - coord_converted.y < 0.1) {
                    obsMesh[i].collisionlInfo |= COLLISION_INFO_BIT_MASK_NORMAL_NORTH;
                }
                else if (obsMesh[i].xyWidth + (double)(0.05f) - coord_converted.x < 0.1) {
                    obsMesh[i].collisionlInfo |= COLLISION_INFO_BIT_MASK_NORMAL_EAST;
                }
                else if (coord_converted.y - (double)(-0.05f) < 0.1) {
                    obsMesh[i].collisionlInfo |= COLLISION_INFO_BIT_MASK_NORMAL_SOUTH;
                }
                else if (coord_converted.x - (double)(-0.05f) < 0.1) {
                    obsMesh[i].collisionlInfo |= COLLISION_INFO_BIT_MASK_NORMAL_WEST;
                }
                else {
                    obsMesh[i].collisionlInfo |= COLLISION_INFO_BIT_MASK_INSIDE_RECT;
                }
            }

            // printf("  __ col info 2: 0x%2x\n\n", obsMesh[i].collisionlInfo);
        }
        else {
            // obsMesh[i].collisionlInfo = 0;
            // printf("  __ col info 3: 0x%2x\n\n", obsMesh[i].collisionlInfo);
            
            // Bouces against the wall in vs2019
            //     which means the absolute value of the normal returned is > 0 ???, a bug ???
            //     weird bugs in this way ??? fixed, used wrong in testLayer.cpp
            obsMesh[i].collisionlInfo &= ~COLLISION_INFO_BIT_MASK_COLLISION_VALID;
            // printf("  __ outide mesh[%d]\n", i);
            // printf("  __ col info: 0x%2x\n\n", obsMesh[i].collisionlInfo);
        }
    }

    // obsMeshIdx = MAX_NUM_OBSTACLE_MESH;
    // return 0;
    return result;
}
