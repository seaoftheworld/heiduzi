// The data in this file is supposed to be read in from a file in the future

// 01. Obstacle cubes data
typedef struct {
    float w, h, rotZ;

    glm::vec3 leftDown;
    glm::vec3 center;
} areaData;

// type-b of obsCubes arrangement
areaData typeB_obsAreas_frontYard[] = {
    //    w,     h,     rot,        leftDown,          center
    { 16.0f,  1.0f,    0.0f, glm::vec3(0.0f), glm::vec3(  8.0f, -0.5f, 0.0f) },  // 1
    { 16.0f,  1.0f,    0.0f, glm::vec3(0.0f), glm::vec3(  8.0f,  8.5f, 0.0f) },

    {  1.0f, 10.0f,    0.0f, glm::vec3(0.0f), glm::vec3( -0.5f,  4.0f, 0.0f) },  // 3
    {  1.0f, 10.0f,    0.0f, glm::vec3(0.0f), glm::vec3( 16.5f,  4.0f, 0.0f) },
    
    {  3.0f, 3.75f,    0.0f, glm::vec3(0.0f), glm::vec3(  5.5f, 6.125f, 0.0f) },  // 5
    {  3.0f, 3.75f,    0.0f, glm::vec3(0.0f), glm::vec3( 12.0f, 6.125f, 0.0f) },

    {  0.5f,  5.0f,   0.0f, glm::vec3(0.0f), glm::vec3( 3.75f,  5.5f, 0.0f) },  // 7 - 10
    {  0.5f,  5.0f,   0.0f, glm::vec3(0.0f), glm::vec3( 7.25f,  5.5f, 0.0f) },
    {  0.5f,  2.0f,  0.0f, glm::vec3(0.0f), glm::vec3( 3.75f, 0.15f, 0.0f) },
    {  0.5f,  2.0f,  0.0f, glm::vec3(0.0f), glm::vec3( 7.25f, 0.15f, 0.0f) },
    
    {}
};

// type-a of obsCubes arrangement, player can be stuck at obsCubes' intersection at first,
// no longer stuck there after the software design's upgraded.
areaData typeA_obsAreas_frontYard[] = {
    //    w,     h,     rot,        leftDown,                         center
    { 16.0f,  1.0f,    0.0f, glm::vec3(0.0f), glm::vec3(  8.0f, -0.5f, 0.0f) },  // 1
    { 16.0f,  1.0f,    0.0f, glm::vec3(0.0f), glm::vec3(  8.0f,  8.5f, 0.0f) },

    {  1.0f, 10.0f,    0.0f, glm::vec3(0.0f), glm::vec3( -0.5f,  4.0f, 0.0f) },  // 3
    {  1.0f, 10.0f,    0.0f, glm::vec3(0.0f), glm::vec3( 16.5f,  4.0f, 0.0f) },
    
    {  4.0f,  0.5f,    0.0f, glm::vec3(0.0f), glm::vec3(  5.5f, 4.0f, 0.0f) },  // 5
    {  3.0f,  0.5f,    0.0f, glm::vec3(0.0f), glm::vec3( 12.0f, 4.0f, 0.0f) },

    {  0.5f,  3.75f,   0.0f, glm::vec3(0.0f), glm::vec3( 3.75f, 6.125f, 0.0f) },  // 7 - 10
    {  0.5f,  3.75f,   0.0f, glm::vec3(0.0f), glm::vec3( 7.25f, 6.125f, 0.0f) },
    {  0.5f,  3.75f,   0.0f, glm::vec3(0.0f), glm::vec3(10.75f, 6.125f, 0.0f) },
    {  0.5f,  3.75f,   0.0f, glm::vec3(0.0f), glm::vec3(13.25f, 6.125f, 0.0f) },

    {  1.0f,  1.0,   __PI_4, glm::vec3(0.0f), glm::vec3( 2.0f, 2.0f, 0.0f) },
    // {  1.0f,  1.0,   __PI_4, glm::vec3(0.0f), glm::vec3( -0.1f, -0.1f, 0.0f) },
    
    {}
};

// These are obsCubes inside plane-01 after the player 'get down stairs' 
// at the trigger of test plane-00
areaData obsAreas_plane01[] = {
    {  0.5f,  3.75f,   0.0f, glm::vec3(0.0f), glm::vec3( 4.0f, 6.125f, -1.0f) },
    {  0.5f,  3.75f,   0.0f, glm::vec3(0.0f), glm::vec3(13.0f, 6.125f, -1.0f) },
    {  8.5f,  1.0f,    0.0f, glm::vec3(0.0f), glm::vec3( 8.5f, 8.5f, -1.0f) },
    {  8.5f,  1.0f,   0.0f, glm::vec3(0.0f), glm::vec3( 8.5f, 3.75f, -1.0f) },
    {}
};


// 02. Triggers data
struct moveVectorSpeed {
    float x, y, z, s;
};
struct roomPosRot {
    float x, y, z, r;
};
struct triggerInfo {  // moved to transferData
    float valid_angle;
    float anim_time;             // use animation (and script ???) to control how the player
    unsigned short anim_repeat;  // gets from 1 plane to another (eg. get up/down stairs)

    roomPosRot pos_rot;
    unsigned char targetRoomIdx;
    unsigned char targetCameraIdx;

    // Data for trigger_ChangePlane
    moveVectorSpeed vec_spd;
    unsigned char targetPlaneIdx;
};

typedef struct {
    const char *type;
    areaData area;
    triggerInfo info;
} triggerData;

roomPosRot backyardToStreetPosRot = {  9.0f,  7.5f, 0.0f, __PI };
roomPosRot streetToBackyardPosRot = { -6.6f, -1.2f, 0.0f, 0.0f };
roomPosRot dummyRoomPosRot = {0.0f, 0.0f, 0.0f, 0.0f};

moveVectorSpeed porch_pl00_down_facing_pX = { 1.0f, 0.0f, -1.0f, 1.414f};  // +X, -Z
moveVectorSpeed porch_pl00_down_facing_mX = {-1.0f, 0.0f, -1.0f, 1.414f};  // -X, -Z
moveVectorSpeed porch_pl01_up_facing_mX =   {-1.0f, 0.0f,  1.0f, 1.414f};  // -X, +Z
moveVectorSpeed porch_pl01_up_facing_pX =   { 1.0f, 0.0f,  1.0f, 1.414f};  // +X, +Z
moveVectorSpeed dummyMoveVecSpd = {0.0f, 0.0f, 0.0f, 0.0f};

triggerData trgPads_frontYard[] = {
    {
        TRIGGER_TYPE_CHANGE_PLANE,
        { 0.25f, 3.5f, 0.0f, glm::vec3(0.0f), glm::vec3( 3.375f + 0.1, 6.125f, 0.0f) }, 
        { -__PI_2, 1.0f, 1, dummyRoomPosRot, backYard, 0, porch_pl00_down_facing_pX, 1 }
    },
    {
        TRIGGER_TYPE_CHANGE_PLANE,
        { 0.25f, 3.5f, 0.0f, glm::vec3(0.0f), glm::vec3(13.625f - 0.1, 6.125f, 0.0f) },
        {  __PI_2, 1.0f, 1, dummyRoomPosRot, backYard, 0, porch_pl00_down_facing_mX, 1 }
    },
    // {
    //     TRIGGER_TYPE_OPEN_DOOR,
    //     { 2.0f, 1.2f, 0.0f, glm::vec3(0.0f), glm::vec3(9.0f, 8.5f, 0.0f) },
    //     // { 0, 0, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f }
    //     {  }
    // },
    {
        TRIGGER_TYPE_OPEN_DOOR,
        { 2.0f, 1.2f, 0.0f, glm::vec3(0.0f), glm::vec3(1.5f, -0.5f, 0.0f) },
        { __PI, 1.0f, 1, backyardToStreetPosRot, theStreet, 0, dummyMoveVecSpd, 0 }
    },
    {  }
};

// These are trigger pads inside plane-01 after the player 'get down stairs' 
// at the trigger of test plane-00
triggerData trgPads_plane01[] = {
    {
        TRIGGER_TYPE_CHANGE_PLANE,
        { 0.25f, 3.5f, 0.0f, glm::vec3(0.0f), glm::vec3( 4.375f - 0.1f, 6.125f, -1.0f) }, 
        {  __PI_2, 1.0f, 1, dummyRoomPosRot, 0, 0, porch_pl01_up_facing_mX, 0 }
    },
    {
        TRIGGER_TYPE_CHANGE_PLANE,
        { 0.25f, 3.5f, 0.0f, glm::vec3(0.0f), glm::vec3(12.625f + 0.1f, 6.125f, -1.0f) },
        { -__PI_2, 1.0f, 1, dummyRoomPosRot, 0, 0, porch_pl01_up_facing_pX, 0 }
    },
    {  }
};

// 03. Transfer data
// struct transferData {
//     areaData area;
//     unsigned short transAnimTtime;   // use animation (and script ???) to control how the player
//     unsigned short transAnimRepeat;  // gets from 1 plane to another (eg. get up/down stairs)
//     float x, y, z;
//     float speed;
//     float angle;
//     unsigned char targetPlaneIdx;
// };

// transferData test00_transfersInFrontYardP00[] = {
//     {
//         { 1.0f, 3.0f, 0.0f, glm::vec3(0.0f), glm::vec3( 3.75f, 6.125f, 0.0f) }, 
//         1, 1, 1.0f, 0.0f, -1.0f, 2.0f, -3.14f / 2.0f, 1
//     },
//     {
//         { 1.0f, 3.0f, 0.0f, glm::vec3(0.0f), glm::vec3(13.25f, 6.125f, 0.0f) },
//         1, 1, -1.0f, 0.0f, -1.0f, 2.0f, 3.14f / 2.0f, 1
//     },
//     { }
// };
// transferData test00_transfersInFrontYardP01[] = {
//     {
//         { 1.0f, 3.0f, 0.0f, glm::vec3(0.0f), glm::vec3( 3.75f, 6.125f, 0.0f) }, 
//         1, 1, 1.0f, 0.0f, -1.0f, 2.0f, -3.14f / 2.0f, 0
//     },
//     {
//         { 1.0f, 3.0f, 0.0f, glm::vec3(0.0f), glm::vec3(13.25f, 6.125f, 0.0f) },
//         1, 1, -1.0f, 0.0f, -1.0f, 2.0f, 3.14f / 2.0f, 0
//     },
//     { }
// };


// 03. camera data
struct camSwitchData {
    areaData area;
    unsigned char targetIndex;
};
struct camPosRotData {
    float x, y, z, hRot, vRot;
};
struct cameraData {
    // unsigned char index;
    // unsigned char numSwitches;
    camSwitchData *sw;
    camPosRotData pos_rot;
};

// camPosRotData dummyCamPosRot = { 14.58f,  -11.4f,  9.6f, -13.4f, -0.54075f };
// camSwitchData dummyCamSw = {};
// cameraData  dummyCam = { &dummyCamSw, dummyCamPosRot };
// cameraData *dummyCams[] = {
//     &dummyCam,
//     NULL
// };


// 04. Planes that contain 'obstacle cubes', 'triggers', and 'transfers'
typedef struct {
    areaData *obsAreas;
    triggerData *trgAreas;
    // transferData *trfAreas;

    cameraData **cam;
} planeData;


// 05. scene data containing planes

// samplePorchData (used for testing)
// planeData plane00 = { test00_obsAreasInBedRoom };
// planeData plane01 = { obsAreas_plane01 };
// planeData* planeInScene[] = {
//     &plane00,
//     &plane01,
//     NULL
// };
// planeData test00_Plane = {
//     test00_obsAreasInBedRoom, test00_trgPadsOnPlane00, dummyCams
// };
// planeData *test00_Planes[] = {
//     &test00_Plane,
//     NULL
// };

planeData test00_Plane[] = {
    // Plane 00
    { typeB_obsAreas_frontYard, trgPads_frontYard, NULL },
    // Plane 01
    { obsAreas_plane01, trgPads_plane01, NULL }
};
planeData *test00_Planes[] = {
    &test00_Plane[0],
    &test00_Plane[1],
    NULL
};
typedef struct {
    planeData **plane = test00_Planes;
} test00_Data;

planeData test01_Plane[] = {
    // Plane 00
    { typeA_obsAreas_frontYard, trgPads_frontYard, NULL },
    // Plane 01
    { obsAreas_plane01, trgPads_plane01, NULL }
};
planeData *test01_Planes[] = {
    &test01_Plane[0],
    &test01_Plane[1],
    NULL
};
typedef struct {
    planeData **plane = test01_Planes;
} test01_Data;


// sampleStreetData
areaData test000_streetObsAreas[] = {
    //   w,     h,             rot,                       leftDown,          center
    {16.5f,  1.0f,            0.0f, glm::vec3(-7.5f,   2.5f, 0.0f), glm::vec3(0.0f)},
    { 1.0f, 15.5f,            0.0f, glm::vec3(-7.5f, -12.5f, 0.0f), glm::vec3(0.0f)},
    { 1.2f,  1.3f, -(3.14f / 6.0f), glm::vec3( 1.4f,  -4.8f, 0.0f), glm::vec3(0.0f)},
    { 7.8f,  2.8f,            0.0f, glm::vec3(-0.3f, -11.7f, 0.0f), glm::vec3(0.0f)},
    {}
};

triggerData test000_streetTrgPads[] = {
    {
        TRIGGER_TYPE_OPEN_DOOR,
        { 1.0f, 1.2f, 0.0f, glm::vec3(0.0f), glm::vec3(9.0f, 8.5f, 0.0f) },
        { 0.0f, 1.0f, 1, streetToBackyardPosRot, backYard, 0, dummyMoveVecSpd, 0 }
    },
    {}
};

camPosRotData streetDummyCamPosRot = { 14.58f,  -11.4f,  9.6f, -13.4f, -0.54075f };
camSwitchData streetDummyCamSw = {};
cameraData streetDummyCam = { &streetDummyCamSw, streetDummyCamPosRot };
cameraData *test000_streetCams[] = {
    &streetDummyCam,
    NULL
};

planeData streetPlane = { test000_streetObsAreas, test000_streetTrgPads, test000_streetCams };
planeData *streetPlanes[] = {
    &streetPlane,
    NULL
};

typedef struct {
    planeData **plane = streetPlanes;
} sampleStreetData;
