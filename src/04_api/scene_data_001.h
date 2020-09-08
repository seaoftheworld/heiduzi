// The data in this file is supposed to be read in from a file in the future

// 01. Obstacle cubes data
// typedef struct {
//     float w, h, rotZ;

//     glm::vec3 leftDown;
//     glm::vec3 center;
// } areaData;

areaData test001_obsAreasInBackYard[] = {
    //    w,     h,     rot,        leftDown,                         center
    {  3.6f, 1.2f,    0.0f, glm::vec3(0.0f, 0.0f, 0.0f),   glm::vec3( 8.65f, 4.10f, 0.0f) },
    {  4.6f, 1.2f,    0.0f, glm::vec3(0.0f, 0.0f, 0.0f),   glm::vec3( 4.74f, 4.48f, 0.0f) },
    { 10.4f, 1.2f,    0.0f, glm::vec3(0.0f, 0.0f, 0.0f),   glm::vec3(-2.56f, 4.11f, 0.0f) },
    {  0.4f, 6.0f,    0.0f, glm::vec3(0.0f, 0.0f, 0.0f),   glm::vec3(-7.54f, 1.00f, 0.0f) },
    { 18.4f, 0.7f,    0.0f, glm::vec3(0.0f, 0.0f, 0.0f),   glm::vec3( 1.50f, -1.8f, 0.0f) },
    {  0.6f, 4.0f,    0.0f, glm::vec3(0.0f, 0.0f, 0.0f),   glm::vec3( 9.40f, 3.00f, 0.0f) },
    {  0.8f, 5.5f,    0.0f, glm::vec3(0.0f, 0.0f, 0.0f),   glm::vec3( 10.0f, 0.96f, 0.0f) },
    
    // -17.5 * 3.14 / 180 == -0.30527778
    {  1.5f, 1.2f, -0.305f, glm::vec3(0.0f, 0.0f, 0.0f),   glm::vec3(-4.80f, -1.44f, 0.0f) },
    // -48 * 3.14 / 180 == -0.83733333
    {  1.2f, 1.2f,  -0.80f, glm::vec3(0.0f, 0.0f, 0.0f),   glm::vec3( 4.28f, -0.82f, 0.0f) },
    
    { 0.16f, 0.12f,    0.0f, glm::vec3(0.0f, 0.0f, 0.0f),   glm::vec3( 8.00f,  0.65f, 0.0f) },
    { 0.16f, 0.12f,    0.0f, glm::vec3(0.0f, 0.0f, 0.0f),   glm::vec3(-3.50f,  0.65f, 0.0f) },
    
    // 5 * 3.14 / 180 == 0.08722222
    { 0.8f, 2.8f,    0.087f, glm::vec3(0.0f, 0.0f, 0.0f),   glm::vec3(-0.62f, 3.05f, 0.0f) },
    // -1.8 * 3.14 / 180 == -0.0314
    { 2.8f, 2.2f,   -0.0314f, glm::vec3(0.0f, 0.0f, 0.0f),   glm::vec3( 1.05f, 2.73f, 0.0f) },

    { 2.7f, 2.7f,    0.044f, glm::vec3(0.0f, 0.0f, 0.0f),   glm::vec3(-6.25f, 3.00f, 0.0f) },
    
    {  }
};



// 02. Triggers data
// struct moveVectorSpeed {
//     float x, y, z, s;
// };
// struct roomPosRot {
//     float x, y, z, r;
// };
// struct triggerInfo {  // moved to transferData
//     float valid_angle;
//     float anim_time;             // use animation (and script ???) to control how the player
//     unsigned short anim_repeat;  // gets from 1 plane to another (eg. get up/down stairs)

//     roomPosRot pos_rot;
//     unsigned char targetRoomIdx;

//     // Data for trigger_ChangePlane
//     moveVectorSpeed vec_spd;
//     unsigned char targetPlaneIdx;
// };

// typedef struct {
//     const char *type;
//     areaData area;
//     triggerInfo info;
// } triggerData;

// roomPosRot porchGatePosRot = {9.0f, 7.5f, 0.0f, __PI};
// roomPosRot porchDoorPosRot = {1.5f, 0.4f, 0.0f, 0.0f};
// roomPosRot dummyRoomPosRot = {0.0f, 0.0f, 0.0f, 0.0f};

// moveVectorSpeed porch_pl00_down_facing_pX = { 1.0f, 0.0f, -1.0f, 1.414f};  // +X, -Z
// moveVectorSpeed porch_pl00_down_facing_mX = {-1.0f, 0.0f, -1.0f, 1.414f};  // -X, -Z
// moveVectorSpeed porch_pl01_up_facing_mX =   {-1.0f, 0.0f,  1.0f, 1.414f};  // -X, +Z
// moveVectorSpeed porch_pl01_up_facing_pX =   { 1.0f, 0.0f,  1.0f, 1.414f};  // +X, +Z
// moveVectorSpeed dummyMoveVecSpd = {0.0f, 0.0f, 0.0f, 0.0f};

triggerData test001_trgPadsInBackYard[] = {
    {
        TRIGGER_TYPE_OPEN_DOOR,
        { 1.2f, 0.5f, 0.0f, glm::vec3(0.0f), glm::vec3(-6.6f, -1.5f, 0.0f) },  // area
        { __PI, 1.0f, 1, porchGatePosRot, theYard, dummyMoveVecSpd, 0 }        // trigger info
    },
    {  }
};



// 03. camera data
// struct camSwitchData {
//     areaData area;
//     unsigned char targetIndex;
// };
// struct cameraData {
//     unsigned char index;
//     camSwitchData *sw;
//     // unsigned char numSwitches;
// };

// camSwitchData cam00_to_01 = { {}, 1 };
// camSwitchData cam01_to_00 = { {}, 0 };
// camSwitchData cam01_to_02 = { {}, 2 };
// camSwitchData cam02_to_01 = { {}, 1 };
//
// camSwitchData cam00sw[] = { {{}, 1} };
// camSwitchData cam01sw[] = { {{}, 0}, {{}, 1} };
// camSwitchData cam02sw[] = { {{}, 1} };
//
camSwitchData cam00sw[] = { {{}, 1}, {} };
camSwitchData cam01sw[] = { {{}, 0}, {{}, 2}, {} };  // to 0/2 when in cam-1
camSwitchData cam02sw[] = { {{}, 1}, {{}, 3}, {} };  // to 1/3 when in cam-2
camSwitchData cam03sw[] = { {{}, 2}, {} };

cameraData test001_camsInBackYard[] = {
    { 0, cam00sw },
    { 1, cam01sw },
    { 2, cam02sw },
    { 3, cam03sw },
    {}
};



// 04. Planes that contain 'obstacle cubes', 'triggers', and 'transfers'
// typedef struct {
//     areaData *obsAreas;
//     triggerData *trgAreas;
//     // transferData *trfAreas;
// } planeData;

// planeData aPlane[] = {
//     // Plane 00
//     // { test00_obsAreasInBedRoom, test00_trgPadsOnPlane00 },
//     // { test02_obsAreasInFrontYard, test01_trgPadsInFrontYard },
//     { test001_obsAreasInBackYard, test001_trgPadsInBackYard },

//     // Plane 01
//     { test01_obsAreasOnPlane01, test01_trgPadsOnPlane01 }
// };
// planeData *apPlane[] = {
//     &aPlane[0],
//     &aPlane[1],
//     NULL
// };
planeData test001_planeForBackYard = { 
    test001_obsAreasInBackYard, 
    test001_trgPadsInBackYard, 
    test001_camsInBackYard 
};
planeData *planeInTheScene[] = {
    &test001_planeForBackYard,
    NULL
};

// 04. scene data containing planes
typedef struct {
    planeData **plane = planeInTheScene;
} test001_backYardData;
