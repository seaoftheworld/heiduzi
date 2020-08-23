#include "testLayer.h"

#include "02_utils/matrices.hpp"  // to call computeMatricesFromInputs() in update() (for debugging)

#include "04_api/actor.h"
#include "04_api/renderer.h"
#include "04_api/scene.h"

#include <string>


testRenderer roomRenderer;  // Renderer for the room of this layer
testRenderer playerRenderer;


glm::vec3 player_startPosition(1.0f, 1.0f, 0.0f);
float     player_startAngle = - 3.14f / 4;
float     player_speeds[actor::speedIdx::max] = { 2.5, 2.0, 1.5 };
actor player(player_startPosition, player_startAngle, player_speeds);

void test_gameLayer::attach() {  // Called by layerManager when this layer is pushed into the queue
    
    testRenderer::printInfo();

    attach00();
    attach01();
    attach02();
}

void test_gameLayer::dettach() {  // Called by layerManager when this layer is poped outof the queue
    dettach02();
    dettach01();
    dettach00();
}

void test_gameLayer::attach00() {
    // Set the gpu Program with correct shaders for rendering this layer
    testRenderer::setTestGpuProgram();
        testRenderer::getUniformLocation_MVP_Mat();
        testRenderer::getUniformLocation_Texture();
    // testRenderer::useTestGpuProgram();

    _dbgSetProgram();

    // openGL ES 2.0 has no VAO !!! ???
    // roomRenderer.allocateVAO();
    // roomRenderer.bindVAO();

    // playerRenderer.allocateVAO();
    // playerRenderer.bindVAO();
}

void test_gameLayer::attach01() {
    // Load the model and texture files for a room/player/3d-obj...
    // m_room = new room(roomIndex::thePorch);
    // roomRenderer.allocateBuffers(m_room->m_ObjModel, m_room->m_PngTexture);  // the texture for the room's 3d-model is also generated here

    const std::string &roomMesh    = "assets/rooms/thePorch/low_poly_room_002.obj";
    const std::string &roomTexture = "assets/rooms/thePorch/low_poly_room_002.png";
    roomRenderer.allocateBuffers(roomMesh, roomTexture);
    
    const std::string &playerMesh    = "assets/rb73_yForward_zUp.obj";
    const std::string &playerTexture = "assets/rb73_base_color.png";
    playerRenderer.allocateBuffers(playerMesh, playerTexture);
}

void test_gameLayer::attach02() {
    // roomRenderer.allocateTextureSlot(m_room, slot);

    roomRenderer.setTextureSlot(1);
    roomRenderer.bindTextureToSlot();

    playerRenderer.setTextureSlot(0);
    playerRenderer.bindTextureToSlot();
}

void test_gameLayer::dettach02() {
    // for slotIdx = 0; < max/used_slot_number; ++
    // roomRenderer.freeTextureSlot(slotIdx);
}

void test_gameLayer::dettach01() {
    playerRenderer.freeBuffers();
    roomRenderer.freeBuffers();
    
    // delete m_room;
}

void test_gameLayer::dettach00() {
    // playerRenderer.unbindVAO();
    // playerRenderer.freeVAO();

    // roomRenderer.unbindVAO();
    // roomRenderer.freeVAO();

    // Delete the gpu Program with correct shaders for rendering this layer
    testRenderer::unsetProgram();

    _dbgUnsetProgram();
}

static glm::vec3 positionCorrection(float delta, glm::vec3 to, glm::vec3 from) {
    
    glm::vec3 des = to;
    unsigned char numObsCubeNear = scene_NearObsCube(delta, des);

    if (numObsCubeNear == 0) {
        // printf("   pos-correction b-0: no obs touched.\n");
        return des;
    }

    glm::vec3 normal = scene_NormalFromObsCube();
    printf("\n\n   num obs near: %d\n", numObsCubeNear);
    printf("   hitNoraml: %f, %f\n", normal.x, normal.y);

    // if (normal == glm::vec3(0.0f)) {
    //     normal = sceneComboNormalFromObsCube();
    // }

    glm::vec3 deltaXYZ = to - from;
    glm::vec3 deltaXYZ__ = deltaXYZ - normal * (glm::dot(deltaXYZ, normal));
    des = from + deltaXYZ__;

    // glm::vec2 coord = coordConvert2d(glm::vec2(des.x, des.y), glm::vec2(3.5f, 3.75f), 0);  // cube[4]
    // printf("  __des on cube[4] coord: %f, %f\n", coord.x, coord.y);


    if ( scene_InObsCube(delta, des) == 0 ) {
        printf("   pos-correction b-1: correction valid, %f, %f.\n\n\n", des.x, des.y);
        return des;
    }
    else {
        printf("   pos-correction b-2: correction aborted.\n\n\n");
        return from;
    }
}

void test_gameLayer::update(float deltaTime, gameInputStatus input) {

    static unsigned char loadNewScene = 1;
    if (loadNewScene) {

        // room r = thePorch;
        scene_LoadData(thePorch);
        scene_SetCurrentPlane(0);

        loadNewScene = 0;
    }

    // Update the mvp(shall be view-mat only) matrixes for the room accoridng to mouse/key input
    computeMatricesFromInputs();

    // Update player's position/angle data based on input    
    // if (input.m_byte) 
    {
        if (input.m_byte & BITMASK_UP) {
            glm::vec3 deltaXY = moveForward(&player, deltaTime);  // model-mat transformed/rotated in renderer
            player.position = positionCorrection(deltaTime, player.position + deltaXY, player.position);
        }
        if (input.m_byte & BITMASK_DOWN) {
            glm::vec3 deltaXY = moveBackward(&player, deltaTime);
            player.position = positionCorrection(deltaTime, player.position + deltaXY, player.position);
        }
        if (input.m_byte & BITMASK_LEFT) {
            turnLeft(&player, deltaTime);
        }
        if (input.m_byte & BITMASK_RIGHT) {
            turnRight(&player, deltaTime);
        }
        
        // printf("%d ",   (input.m_byte & BITMASK_TRIGGER) ? (1) : (0) );
        // printf("%d ",   (input.m_byte & BITMASK_FIRE) ? (1) : (0) );
        // printf("%d.\n", (input.m_byte & BITMASK_INVENTORY) ? (1) : (0) );
    }

    // Call each trigger's updater method one by one
    scenePlane* plane = scene_GetCurrentPlane();
    for (unsigned short i = 0; i < plane->numTriggers; i++) {
        triggerPad *trigger = &plane->trigger[0] + i;

        // printf("  trigger:%d update() called.\n", i);
        trigger->update(deltaTime);
    }
}

void test_gameLayer::render() {
    /*
    // This part is a test for the room logic
    if (m_room) {
        if (m_room->isDoorOpened()) {
            roomIndex idx = room::toTheNextRoom(m_room);

            delete m_room;
            m_room = new room(idx);

            roomRenderer.freeBuffers();
            roomRenderer.allocateBuffers(m_room->m_ObjModel, m_room->m_PngTexture);

            // slot = room->texture_slot()
            // if slot == Max (or < 0)
                // ++slotIdx
                // if ( slotIdx == Max ) 
                //     slotIdx = 0; 
                // freeTextureSlot(slotIdx)
                // roomRenderer.allocateTextureSlot(m_room, slotIdx);
                // room->set_texture_slot(slotIdx)
            // else
                // slotIdx = slot
        }
    }
    //*/

    // To draw debug lines for obsCubes and triggers in the current plane
    scenePlane *plane = scene_GetCurrentPlane();
    for (unsigned char i = 0; i < plane->numObsCubes; i++) {

        cube *obsCube = &plane->obsCube[i];
        // if ( (obsCube->bottomRect.collisionInfo & COLLISION_INFO_BIT_MASK_COLLISION_VALID) != 0 ) {
        if (1) {

            float vt[NUM_VERTICES_FOR_CUBE * 3] = { 0 };
            for (unsigned char j = 0; j < NUM_VERTICES_FOR_CUBE; j++) {
                vt[j * 3 + 0] = obsCube->vertices[j].x;
                vt[j * 3 + 1] = obsCube->vertices[j].y;
                vt[j * 3 + 2] = obsCube->vertices[j].z;
            }

            _dbgDrawBox(vt);
        }
    }
    for (unsigned char i = 0; i < plane->numTriggers; i++) {
        cube *trgCube = &plane->trigger[i].pad;

        if (1) {
            float vt[NUM_VERTICES_FOR_CUBE * 3] = { 0 };
            for (unsigned char j = 0; j < NUM_VERTICES_FOR_CUBE; j++) {
                vt[j * 3 + 0] = trgCube->vertices[j].x;
                vt[j * 3 + 1] = trgCube->vertices[j].y;
                vt[j * 3 + 2] = trgCube->vertices[j].z;
            }

            _dbgDrawBox(vt);
        }
    }

    testRenderer::useTestGpuProgram();
    // 2.1 Set the slot to be used for the texture of the room, and draw the room with buffers
        // roomRenderer.useTextureSlot();
        // roomRenderer.drawBuffers();           // draw the room with previously updated V/P matrixes, 
        //                                       // shall draw with updated 'view' data in the future... ...

    // 2.2 Set the slot to be used for the texture of the player, and draw the player with buffers
    playerRenderer.useTextureSlot();
    playerRenderer.drawBuffers(&player);  // draw the player with previously updated position/angle data
}

    /*
    // To draw debug lines
    for (unsigned char i = 0; i < scene_00.numObstacleMesh; i++) {

        if ( (scene_00.obsMesh[i].collisionlInfo & COLLISION_INFO_BIT_MASK_COLLISION_VALID) != 0 ) {
            // float vt[NUM_VERTICES_FOR_BOX][3];
            // for (unsigned char j = 0; j < NUM_VERTICES_FOR_BOX; j++) {
            //     vt[j][0] = scene_00.obsMesh[i].vertices[j].x;
            //     vt[j][1] = scene_00.obsMesh[i].vertices[j].y;
            //     vt[j][2] = scene_00.obsMesh[i].vertices[j].z;
            // }

            // // printf("  _dbg_drawBox() called... ...\n");
            // testRenderer::_dbg_useProgram();
            // _dbgDrawBox(&vt[0]);

            float vt[NUM_VERTICES_FOR_BOX * 3] = {0};
            for (unsigned char j = 0; j < NUM_VERTICES_FOR_BOX; j++) {
                vt[j * 3 + 0] = scene_00.obsMesh[i].vertices[j].x;
                vt[j * 3 + 1] = scene_00.obsMesh[i].vertices[j].y;
                vt[j * 3 + 2] = scene_00.obsMesh[i].vertices[j].z;
            }

            _dbgDrawBox(vt);
        }
    }
    //*/
