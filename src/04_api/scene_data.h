// The data in this file is supposed to be read in from a file in the future

// 01. Obstacle cubes data
typedef struct {
    float w, h, rotZ;

    glm::vec3 leftDown;
    glm::vec3 center;
} areaData;

areaData test00_obsAreasInBedRoom[] = {
    //   w,     h,             rot,                       leftDown,          center
    {16.5f,  1.0f,            0.0f, glm::vec3(-7.5f,   2.5f, 0.0f), glm::vec3(0.0f)},
    { 1.0f, 15.5f,            0.0f, glm::vec3(-7.5f, -12.5f, 0.0f), glm::vec3(0.0f)},
    { 1.2f,  1.3f, -(3.14f / 6.0f), glm::vec3( 1.4f,  -4.8f, 0.0f), glm::vec3(0.0f)},
    { 7.8f,  2.8f,            0.0f, glm::vec3(-0.3f, -11.7f, 0.0f), glm::vec3(0.0f)},
    // {    0,     0,            0.0f, glm::vec3( 0.0f,   0.0f, 0.0f), glm::vec3(0.0f)}
    {  }
};
areaData test01_obsAreasInFrontYard[] = {
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
    
    {  }
};
areaData test02_obsAreasInFrontYard[] = {
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
    
    {  }
};
areaData test01_obsAreasOnPlane01[] = {
    {  0.5f,  3.75f,   0.0f, glm::vec3(0.0f), glm::vec3( 4.0f, 6.125f, -1.0f) },
    {  0.5f,  3.75f,   0.0f, glm::vec3(0.0f), glm::vec3(13.0f, 6.125f, -1.0f) },
    {  8.5f,  1.0f,    0.0f, glm::vec3(0.0f), glm::vec3( 8.5f, 8.5f, -1.0f) },
    {  8.5f,  1.0f,   0.0f, glm::vec3(0.0f), glm::vec3( 8.5f, 3.75f, -1.0f) },
    {  }
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

    // Data for trigger_ChangePlane
    moveVectorSpeed vec_spd;
    unsigned char targetPlaneIdx;
};

typedef struct {
    const char *type;
    areaData area;
    triggerInfo info;
} triggerData;

triggerData test00_trgPadsOnPlane00[] = {
    {
        TRIGGER_TYPE_CHANGE_PLANE,
        {1.0f, 1.0f, 0.0f, glm::vec3(0.0f), glm::vec3(4.5f, 4.5f, 0.0f)},
        {  }
    },
    {
        TRIGGER_TYPE_CHANGE_PLANE,
        {1.0f, 1.0f, 0.0f, glm::vec3(0.0f), glm::vec3(-1.5f, -1.5f, 0.0f)},
        {  }
    },
    {  }
};

roomPosRot dummyRoomPosRot = {0.0f, 0.0f, 0.0f, 0.0f};
roomPosRot porchGatePosRot = {9.0f, 7.5f, 0.0f, __PI};
roomPosRot porchDoorPosRot = {1.5f, 0.4f, 0.0f, 0.0f};

moveVectorSpeed dummyMoveVecSpd = {0.0f, 0.0f, 0.0f, 0.0f};

moveVectorSpeed porch_pl00_down_facing_pX = { 1.0f, 0.0f, -1.0f, 1.414f};  // +X, -Z
moveVectorSpeed porch_pl00_down_facing_mX = {-1.0f, 0.0f, -1.0f, 1.414f};  // -X, -Z
moveVectorSpeed porch_pl01_up_facing_mX =   {-1.0f, 0.0f,  1.0f, 1.414f};  // -X, +Z
moveVectorSpeed porch_pl01_up_facing_pX =   { 1.0f, 0.0f,  1.0f, 1.414f};  // +X, +Z

triggerData test01_trgPadsInFrontYard[] = {
    {
        TRIGGER_TYPE_CHANGE_PLANE,
        { 0.25f, 3.5f, 0.0f, glm::vec3(0.0f), glm::vec3( 3.375f + 0.1, 6.125f, 0.0f) }, 
        { -__PI_2, 1.0f, 1, dummyRoomPosRot, thePorch, porch_pl00_down_facing_pX, 1 }
    },
    {
        TRIGGER_TYPE_CHANGE_PLANE,
        { 0.25f, 3.5f, 0.0f, glm::vec3(0.0f), glm::vec3(13.625f - 0.1, 6.125f, 0.0f) },
        {  __PI_2, 1.0f, 1, dummyRoomPosRot, thePorch, porch_pl00_down_facing_mX, 1 }
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
        { __PI, 1.0f, 1, porchGatePosRot, theYard, dummyMoveVecSpd, 0 }
    },
    {  }
};
triggerData test01_trgPadsOnPlane01[] = {
    {
        TRIGGER_TYPE_CHANGE_PLANE,
        { 0.25f, 3.5f, 0.0f, glm::vec3(0.0f), glm::vec3( 4.375f - 0.1f, 6.125f, -1.0f) }, 
        {  __PI_2, 1.0f, 1, dummyRoomPosRot, 0, porch_pl01_up_facing_mX, 0 }
    },
    {
        TRIGGER_TYPE_CHANGE_PLANE,
        { 0.25f, 3.5f, 0.0f, glm::vec3(0.0f), glm::vec3(12.625f + 0.1f, 6.125f, -1.0f) },
        { -__PI_2, 1.0f, 1, dummyRoomPosRot, 0, porch_pl01_up_facing_pX, 0 }
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


// 04. Planes that contain 'obstacle cubes', 'triggers', and 'transfers'
typedef struct {
    areaData *obsAreas;
    triggerData *trgAreas;
    // transferData *trfAreas;
} planeData;

// planeData plane00 = { test00_obsAreasInBedRoom };
// planeData plane01 = { test01_obsAreasOnPlane01 };
// planeData* planeInScene[] = {
//     &plane00,
//     &plane01,
//     NULL
// };
planeData aPlane[] = {
    // Plane 00
    // { test00_obsAreasInBedRoom, test00_trgPadsOnPlane00 },
    // { test02_obsAreasInFrontYard, test01_trgPadsInFrontYard },
    { test01_obsAreasInFrontYard, test01_trgPadsInFrontYard },

    // Plane 01
    { test01_obsAreasOnPlane01, test01_trgPadsOnPlane01 }
};
planeData *apPlane[] = {
    &aPlane[0],
    &aPlane[1],
    NULL
};


// 04. scene data containing planes
typedef struct {
    planeData **plane = apPlane;
} samplePorchData;



// ==========================================================
triggerData test01_yardTrgPads[] = {
    {
        TRIGGER_TYPE_OPEN_DOOR,
        { 1.0f, 1.2f, 0.0f, glm::vec3(0.0f), glm::vec3(9.0f, 8.5f, 0.0f) },
        { 0.0f, 1.0f, 1, porchDoorPosRot, thePorch, dummyMoveVecSpd, 0 }
    },
    {  }
};
planeData yardPlane = { test00_obsAreasInBedRoom, test01_yardTrgPads };
planeData *apPlane_01[] = {
    &yardPlane,
    NULL
};

typedef struct {
    planeData **plane = apPlane_01;
} sampleYardData;


// ==========================================================
areaData initObsCube[] = {
    {  }
};
triggerData initTrgPad[] = {
    {
        TRIGGER_TYPE_OPEN_DOOR,
        { 1.0f, 1.0f, 0.0f, glm::vec3(0.0f), glm::vec3(1.0f, 1.0f, 0.0f) },
        { 0.0f, 1.0f, 1, porchGatePosRot, thePorch, dummyMoveVecSpd, 0 }
    },
    {  }
};
planeData initPlane = { initObsCube, initTrgPad };
planeData *init_apPlane[] = {
    &initPlane,
    NULL
};

typedef struct {
    planeData **plane = init_apPlane;
} sampleInitData;


// planeData array_planeInScene[] = {
//     { test00_obsAreasInBedRoom },
//     { test01_obsAreasOnPlane01 },
//     { NULL }
// };
// typedef struct {
//     // When an array of this struct is given, have to
//     // analysis into 'planeData' components, or the 
//     // value of member var in this struct to 
//     // determin the end of the array.
//     // Use 'planeData *[]' datatype instead of an array of this struct.
//     // the end element of the poiners-array is asigned a NULL pointer.
//     //
//     planeData *planes = array_planeInScene;
// }sceneData;

// typedef struct {
//     planeData *planes = apPlane[0];  // If (planeData *) data-type is used,
//                                      // it should be initialized by the name of an array of 'planeData', 
//                                      // not by a name of an array of 'planeData *'
//                                      // Otherwise, (like this line of code), planes++ doesn't iterate through 
//                                      // the rest of pointers in the array as expected, 
//                                      // since it adds by the sizeof(planeData), not by the sizeof(planeData *)
//                                      // To initialize by a name of an array of 'planeData *', 
//                                      // use 'planeData **' data type.
    
//     planeData **plane = apPlane;
//     planeData **p = apPlane;  // p++ iterate through the rest of pointers in the array, 
//                               // p[0], p[1]... works as 'planeData *'

// } sampleSceneData;
