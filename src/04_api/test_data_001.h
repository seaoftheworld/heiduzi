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
    
    // The 2 street lamps
    // { 0.16f, 0.12f,    0.0f, glm::vec3(0.0f, 0.0f, 0.0f),   glm::vec3( 8.00f,  0.65f, 0.0f) },
    // { 0.16f, 0.12f,    0.0f, glm::vec3(0.0f, 0.0f, 0.0f),   glm::vec3(-3.50f,  0.65f, 0.0f) },
    
    // 5 * 3.14 / 180 == 0.08722222
    { 0.8f, 2.8f,    0.087f, glm::vec3(0.0f, 0.0f, 0.0f),   glm::vec3(-0.62f, 3.05f, 0.0f) },
    // -1.8 * 3.14 / 180 == -0.0314
    { 2.8f, 2.2f,   -0.0314f, glm::vec3(0.0f, 0.0f, 0.0f),   glm::vec3( 1.05f, 2.73f, 0.0f) },

    { 2.7f, 2.7f,    0.044f, glm::vec3(0.0f, 0.0f, 0.0f),   glm::vec3(-6.25f, 3.00f, 0.0f) },
    
    {}
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
//     unsigned char targetCameraIdx;

//     // Data for trigger_ChangePlane
//     moveVectorSpeed vec_spd;
//     unsigned char targetPlaneIdx;
// };

// typedef struct {
//     const char *type;
//     areaData area;
//     triggerInfo info;
// } triggerData;
triggerData test001_trgPadsInBackYard[] = {
    {
        TRIGGER_TYPE_OPEN_DOOR,
        { 1.2f, 0.5f, 0.0f, glm::vec3(0.0f), glm::vec3(-6.6f, -1.5f, 0.0f) },        // area
        { __PI, 1.0f, 1, backyardToStreetPosRot, theStreet, 0, dummyMoveVecSpd, 0 }  // trigger info
    },
    {}
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
// areaData dummyArea = {
//     1.0f, 1.0f, 0.0f, glm::vec3(0.0f), glm::vec3(0.5f, 0.5f, 0.0f)  // w, h, rot, leftDown, center
// };
areaData cam00_to_01 = {
    // w,    h,            rot                         center
    1.0f, 3.0f, (3.14f / 12.0f), glm::vec3(0.0f), glm::vec3(-4.42f, 0.53f, 0.0f)
};
areaData cam01_to_00 = {
    1.0f, 3.0f, (3.14f / 12.0f), glm::vec3(0.0f), glm::vec3(-5.44f, 0.23f, 0.0f)  
};
areaData cam01_to_02 = {
    1.0f, 4.0f, 0.0f, glm::vec3(0.0f), glm::vec3(1.1f, 0.15f, 0.0f)  
};
areaData cam02_to_01 = {
    1.0f, 4.0f, 0.0f, glm::vec3(0.0f), glm::vec3(0.0f, 0.15f, 0.0f)  
};
areaData cam02_to_03 = {
    1.0f, 6.0f, -(3.14f / 12.0f), glm::vec3(0.0f), glm::vec3(7.6f, 1.0f, 0.0f)
};
areaData cam03_to_02 = {
    1.0f, 6.0f, -(3.14f / 12.0f), glm::vec3(0.0f), glm::vec3(6.5f, 1.0f, 0.0f)
};

camPosRotData cam00_pos_rot = { -4.47f,   2.57f,  0.28f, -8.96f, -0.075f};
camPosRotData cam01_pos_rot = {  3.86f,   0.02f,  1.00f,  -7.5f,  -0.12f};
camPosRotData cam02_pos_rot = { 8.047f, -0.976f, 0.964f, -7.25f, -0.115f};
camPosRotData cam03_pos_rot = { -1.57f,   -0.6f,  2.72f, -5.32f, -0.555f};

camSwitchData cam00sw[] = { {cam00_to_01, 1}, {} };
camSwitchData cam01sw[] = { {cam01_to_00, 0}, {cam01_to_02, 2}, {} };  // to 0/2 when in cam-1
camSwitchData cam02sw[] = { {cam02_to_01, 1}, {cam02_to_03, 3}, {} };  // to 1/3 when in cam-2
camSwitchData cam03sw[] = { {cam03_to_02, 2}, {} };

cameraData test001_camInBackYard[] = {
    // { cam01sw, { -2.76f,  -3.08f,  5.05f, -6.28f,  -0.94f} },
    // { cam01sw, { 3.41f,    0.91f,  1.35f,  -7.87f,  -0.37f} },
    { cam00sw, cam00_pos_rot },
    { cam01sw, cam01_pos_rot },
    { cam02sw, cam02_pos_rot },
    { cam03sw, cam03_pos_rot },
};

cameraData *test001_camsInBackYard[] = {
    &test001_camInBackYard[0],
    &test001_camInBackYard[1],
    &test001_camInBackYard[2],
    &test001_camInBackYard[3],
    NULL
};



// 04. Planes that contain 'obstacle cubes', 'triggers', and 'transfers'
// typedef struct {
//     areaData *obsAreas;
//     triggerData *trgAreas;
//     cameraData **cam;
// } planeData;
planeData test001_planeForBackYard = { 
    test001_obsAreasInBackYard, test001_trgPadsInBackYard, test001_camsInBackYard 
};
planeData *test001_planesForBackYard[] = {
    &test001_planeForBackYard,
    NULL
};

// 04. scene data containing planes
typedef struct {
    planeData **plane = test001_planesForBackYard;
} SampleBackyardData;
