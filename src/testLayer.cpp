#include "testLayer.h"

#include "02_utils/matrices.hpp"  // to call computeMatricesFromInputs() in update() (for debugging)

#include "04_api/actor.h"
#include "04_api/renderer.h"
#include "04_api/scene.h"

#include <string>


testRenderer roomRenderer;  // Renderer for the room of this layer
testRenderer playerRenderer;

// Player's Init positon/rotation shall be read in from room data
glm::vec3 player_startPosition(1.0f, 1.0f, 0.0f);
float     player_startAngle = - 3.14f / 4;
float     player_speeds[actor::speedIdx::max] = { 2.5, 2.0, 1.5 };
actor player(player_startPosition, player_startAngle, player_speeds);

static std::string roomMesh;
static std::string roomTexture;

static void getModelTexturePath(room rm) {
    switch ( rm ) {
        case thePorch:
            roomMesh    = "assets/rooms/thePorch/rb73_yForward_zUp.obj";
            roomTexture = "assets/rooms/thePorch/low_poly_room_002.png";
        break;

        case theYard:
            // roomMesh    = "assets/rooms/thePorch/rb73_yForward_zUp.obj";
            roomMesh    = "assets/rooms/theYard/low_poly_room_002.obj";
            roomTexture = "assets/rooms/theYard/low_poly_room_002.png";
        break;

        // use test/debug room mesh/texture in this case to show error occured
        case roomMax:
        default:
            roomMesh = "???";
            roomTexture = "???";
        break;
    }
}

static void enterRoomUpdate(targetPlaceInfo targetPlace) {
    scene_LoadData(targetPlace.rm, targetPlace.pl);
    player.position = glm::vec3(targetPlace.enteringPosRot[0], targetPlace.enteringPosRot[1], targetPlace.enteringPosRot[2]);
    player.yAngle = targetPlace.enteringPosRot[3];
}

static glm::vec3 positionCorrection(float delta, glm::vec3 to, glm::vec3 from) {
    
    glm::vec3 des = to;
    unsigned char numObsCubeNear = scene_NearObsCube(delta, des);

    if (numObsCubeNear == 0) {
        // printf("   pos-correction b-0: no obs touched.\n");
        return des;
    }

    glm::vec3 normal = scene_NormalFromNearObsCube();
    printf("\n\n   num obs near: %d\n", numObsCubeNear);
    printf("   hitNoraml: %f, %f, %f\n", normal.x, normal.y, normal.z);

    // if (normal == glm::vec3(0.0f)) {
    //     normal = sceneComboNormalFromObsCube();
    // }

    glm::vec3 deltaXYZ = to - from;
    glm::vec3 deltaXYZ__ = deltaXYZ - normal * (glm::dot(deltaXYZ, normal));
    des = from + deltaXYZ__;

    // glm::vec2 coord = coordConvert2d(glm::vec2(des.x, des.y), glm::vec2(3.5f, 3.75f), 0);  // cube[4]
    // printf("  __des on cube[4] coord: %f, %f\n", coord.x, coord.y);


    if ( scene_InNearObsCube(des) == 0 ) {
        printf("   pos-correction b-1: correction valid, %f, %f.\n\n\n", des.x, des.y);
        return des;
    }
    else {
        printf("   pos-correction b-2: correction aborted.\n\n\n");
        return from;
    }
}



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
    const std::string &roomMesh    = "assets/rooms/thePorch/rb73_yForward_zUp.obj";
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

void test_gameLayer::update(float deltaTime, gameInputStatus input) {

    static unsigned char firstUpdate = 1;
    if (firstUpdate) {
        // const std::string &roomMesh    = "assets/rooms/thePorch/rb73_yForward_zUp.obj";
        // const std::string &roomTexture = "assets/rooms/thePorch/low_poly_room_002.png";
        // roomRenderer.allocateBuffers(roomMesh, roomTexture);

        // roomRenderer.setTextureSlot(1);
        // roomRenderer.bindTextureToSlot();

        // playerRenderer.setTextureSlot(0);
        // playerRenderer.bindTextureToSlot();

        // scene_LoadData(thePorch);  // load with roomIndex, 
        // // and call something like ??? to set player's pos according to some door's trigger data
        // scene_setCurrentPlane(0);  // shall be within the above api call

        // enterRoom(initDoorData);
        //     scene_setPlayer(d.pos, d.rot);
        //     scene_loadRoomData(door.rIdx, &mesh, &texture);
        //     roomRenderer.freeBuffers();
        //     roomRenderer.allocateBuffers(mesh, texture);  // blocks when reading .obj 3d model

        // Nasty trick not good
            // scene_LoadData(roomInit);
            // player.position = glm::vec3(1.0f, 1.0f, 0.0f);
            // player.yAngle = 0.0f;
            // input.m_byte &= ~BITMASK_TRIGGER;
            // input.m_byte |= BITMASK_FIRE;

        // Nasty trick, even worse.
            // scene_LoadData(roomInit);
            // scene_getCurrentPlane()->trigger[0].status |= TRIGGER_STATUS_BIT_MASK_UPDATING;
            // scene_getCurrentPlane()->trigger[0].pos = &player.position;  
            // scene_getCurrentPlane()->trigger[0].rot = &player.yAngle;  
            // setSceneStatus( getSceneStatus() | SCENE_STATUS_BIT_MASK_OPENNING_DOOR);
        
        targetPlaceInfo target;
        target.rm = thePorch;
        target.pl = 0;
        target.enteringPosRot[0] = 9.0f;
        target.enteringPosRot[1] = 7.5f;
        target.enteringPosRot[2] = 0.0f;
        target.enteringPosRot[3] = __PI;
        
        enterRoomUpdate(target);

        firstUpdate = 0;
    }


    // Update the mvp(shall be view-mat only) matrixes for the room accoridng to mouse/key input
    computeMatricesFromInputs();

    scene_updateTriggerAndStatus(deltaTime, &player.position, &player.yAngle, input.m_byte);

    if ( scene_getStatus(SCENE_STATUS_PLAYING_OPEN_DOOR_ANIMATION) || 
         scene_getStatus(SCENE_STATUS_ALLOCATING_VBO_AND_TEXTURE) ) {

        if ( scene_getStatus(SCENE_STATUS_ALLOCATING_VBO_AND_TEXTURE) ) {
            static unsigned char step = 0;
            // printf("  allocating-buffer");
            printf("  a");

            if (step == 0) {
                // Get string for roomMesh/texture from data in scene
                getModelTexturePath( scene_getTargetPlace()->rm );

                roomRenderer.freeBuffers();
                step++;
            }

            if (step == 1) {
                unsigned char alloc_result = roomRenderer.nonblocking_allocateBuffers(roomMesh, roomTexture);
                if (alloc_result == 0) {
                    roomRenderer.setTextureSlot(1); 
                    roomRenderer.bindTextureToSlot();

                    playerRenderer.setTextureSlot(0);
                    playerRenderer.bindTextureToSlot();

                    step++;
                }
            }

            if (step == 2) {
                // Set collision/trigger info, plane, and player's pos/rot

                // Do not load the collision/trigger data for the new room until both
                // allocating-buffer is finished, and all openning-door's anim update() calls
                // are called. When openning door's update() call is not finished yet,
                // loading the new room's data could cause the new room's 1st 
                // trigger's update() to be called instead. 
                // It's a defect from the design. Tobe improved !!!
                //
                if (scene_getStatus() & SCENE_STATUS_PLAYING_OPEN_DOOR_ANIMATION) {
                }
                else {
                    // Not entered until playing open-door animation is finished
                    printf("\n  allocating-buffer finished\n\n");
                    enterRoomUpdate( *scene_getTargetPlace() );
                    
                    scene_setStatus(SCENE_STATUS_ALLOCATING_VBO_AND_TEXTURE, 0);
                    step = 0;
                }
            }
        }
    }

    if (scene_getStatus() & SCENE_STATUS_CHANGE_PLANE_INDEX) {
        scene_setCurrentPlane( scene_getTargetPositionPlane()->plIdx );
    }

    if ( scene_getStatus() & SCENE_STATUS_UPDATE_PLAYER_BY_CHANGE_PLANE ) {
        float *pos = scene_getTargetPositionPlane()->pos;
        player.position += glm::vec3(pos[0], pos[1], pos[2]);
    }
    else if ( scene_getStatus() & SCENE_STATUS_UPDATE_PLAYER_BY_INPUT ) {
        // Read input for player navigation only when no trigger pad is being updated
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
    }

    if ( scene_getStatus(SCENE_STATUS_PLAYING_OPEN_DOOR_ANIMATION) ||
         scene_getStatus(SCENE_STATUS_ALLOCATING_VBO_AND_TEXTURE) ) {
        // set gl clear color to be black
        float black_rgba[] = {0.0f, 0.0f, 0.0f, 1.0f};
        testRenderer::setClearColor(black_rgba);
    }
    else {
        // set gl clear color to be blue
        float blue_rgba[] = {0.0f, 0.0f, 0.4f, 1.0f};
        testRenderer::setClearColor(blue_rgba);
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

    if ( scene_getStatus(SCENE_STATUS_PLAYING_OPEN_DOOR_ANIMATION) ||
         scene_getStatus(SCENE_STATUS_ALLOCATING_VBO_AND_TEXTURE) ) {
        //display door animation ??? ...
        // printf("black screen render\n\n\n");
    }
    else {

        // printf("normal render\n");
        // To draw debug lines for obsCubes and triggers in the current plane
        scenePlane *plane = scene_getCurrentPlane();
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
        roomRenderer.useTextureSlot();
        roomRenderer.drawBuffers();           // draw the room with previously updated V/P matrixes, 
                                              // shall draw with updated 'view' data in the future... ...

        // 2.2 Set the slot to be used for the texture of the player, and draw the player with buffers
        playerRenderer.useTextureSlot();
        playerRenderer.drawBuffers(&player);  // draw the player with previously updated position/angle data
    }
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
