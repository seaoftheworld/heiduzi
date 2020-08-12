#pragma once

// Include 'GLM' to only use 'glm::vec3' ... data type, 
// and glm::translate(), rotate()...
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#define NUM_VERTICES_FOR_BOX  (8)
#define MAX_NUM_OBSTACLE_MESH (32)
#define ZHEIGHT_OBSTACLE_MESH (8.0f)
#define ZHEIGHT ZHEIGHT_OBSTACLE_MESH

#define COLLISION_INFO_BIT_MASK_COLLISION_VALID  (1 << (sizeof(unsigned char) * 8 - 1))
// #define COLLISION_INFO_BIT_MASK_COLLISION_VALID  (1 << (7))
#define COLLISION_INFO_BIT_MASK_NORMAL_NORTH  (1 << (0))
#define COLLISION_INFO_BIT_MASK_NORMAL_EAST   (1 << (1))
#define COLLISION_INFO_BIT_MASK_NORMAL_SOUTH  (1 << (2))
#define COLLISION_INFO_BIT_MASK_NORMAL_WEST   (1 << (3))
#define COLLISION_INFO_BIT_MASK_INSIDE_RECT   (1 << (4))

    
struct obstacleMesh {
    enum normalId{
        north = 0, east, south, west, max
    };

    obstacleMesh() {}
    obstacleMesh(float *wh, glm::vec2 trans, float rotZ);


    glm::vec3 vertices[NUM_VERTICES_FOR_BOX];
    float xyWidth = 0, xyHeight = 0;
    
    glm::vec2 transXY = glm::vec2(0.0f, 0.0f);
    float rotZ = 0;

    glm::vec3 normal[max];
    unsigned char collisionlInfo = 0;
};

struct sceneSystem {
    sceneSystem() {}

    obstacleMesh obsMesh[MAX_NUM_OBSTACLE_MESH];
    unsigned short numObstacleMesh = 0;
    
    static glm::vec2 coordinateConvert(glm::vec2 coord, glm::vec2 trans, float r);

    void loadSceneInfo();
    unsigned char onObstacleRect(glm::vec3 des);
    glm::vec3 normalFromObstacleRect();
};
