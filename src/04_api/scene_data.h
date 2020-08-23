// The data in this file is supposed to be read in from a file in the future

// 01. Obstacle cubes data
typedef struct {
    float w, h, rotZ;

    glm::vec3 leftDown;
    glm::vec3 center;
} cubeData;

cubeData test00_obsCubesInBedRoom[] = {
    //   w,     h,             rot,                       leftDown,          center
    {16.5f,  1.0f,            0.0f, glm::vec3(-7.5f,   2.5f, 0.0f), glm::vec3(0.0f)},
    { 1.0f, 15.5f,            0.0f, glm::vec3(-7.5f, -12.5f, 0.0f), glm::vec3(0.0f)},
    { 1.2f,  1.3f, -(3.14f / 6.0f), glm::vec3( 1.4f,  -4.8f, 0.0f), glm::vec3(0.0f)},
    { 7.8f,  2.8f,            0.0f, glm::vec3(-0.3f, -11.7f, 0.0f), glm::vec3(0.0f)},
    // {    0,     0,            0.0f, glm::vec3( 0.0f,   0.0f, 0.0f), glm::vec3(0.0f)}
    {  }
};
cubeData test01_obsCubesInFrontYard[] = {
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

    {  1.0f,  1.0,   0.0f, glm::vec3(0.0f), glm::vec3( 2.0f, 2.0f, 0.0f) },
    
    {  }
};
cubeData test02_obsCubesInFrontYard[] = {
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
cubeData test00_obsCubesOnPlane01[] = {
    // {    0,     0,            0.0f, glm::vec3( 0.0f,   0.0f, 0.0f), glm::vec3(0.0f)}  
    {  }
};


// 02. Triggers data
typedef struct {
    const char *type;
    cubeData pad;
} triggerPadData;

triggerPadData test00_trgPadsOnPlane00[] = {
    {
        "trigger_ChangePlane",
        {1.0f, 1.0f, 0.0f, glm::vec3(0.0f), glm::vec3(4.5f, 4.5f, 0.0f)}
    },
    {
        "trigger_ChangePlane",
        {1.0f, 1.0f, 0.0f, glm::vec3(0.0f), glm::vec3(-1.5f, -1.5f, 0.0f)}
    },
    { 
    }
};
triggerPadData test00_trgPadsOnPlane01[] = {
    {
        "trigger_ChangePlane",
        {2.0f, 2.0f, 0.0f, glm::vec3(0.0f), glm::vec3(1.0f, 1.0f, 0.0f)}
    },
    // {
    //     "trigger_ChangePlane",
    //     {1.0f, 1.0f, 0.0f, glm::vec3(0.0f), glm::vec3(-0.5f, -0.5f, 0.0f)}
    // },
    { 
    }
};
triggerPadData test01_trgPadsInFrontYard[] = {
    {
        "trigger_ChangePlane",
        { 1.0f, 3.0f, 0.0f, glm::vec3(0.0f), glm::vec3( 3.75f, 6.125f, 0.0f) }
    },
    {
        "trigger_ChangePlane",
        { 1.0f, 3.0f, 0.0f, glm::vec3(0.0f), glm::vec3(13.25f, 6.125f, 0.0f) }
    },
    {
        "trigger_OpenDoor",
        { 2.0f, 1.2f, 0.0f, glm::vec3(0.0f), glm::vec3(9.0f, 8.5f, 0.0f) }
    },
    {
        "trigger_OpenDoor",
        { 2.0f, 1.2f, 0.0f, glm::vec3(0.0f), glm::vec3(1.5f, -0.5f, 0.0f) }
    },
    { 
    }
};


// 03. Planes that contain 'obstacle cubes' and 'triggers'
typedef struct {
    cubeData *obsCubes;
    triggerPadData *trgPads;
} planeData;

// planeData plane00 = { test00_obsCubesInBedRoom };
// planeData plane01 = { test00_obsCubesOnPlane01 };
// planeData* planeInScene[] = {
//     &plane00,
//     &plane01,
//     NULL
// };
planeData aPlane[] = {
    // Plane 00
    // { test00_obsCubesInBedRoom, test00_trgPadsOnPlane00 },
    // { test01_obsCubesInFrontYard, test01_trgPadsInFrontYard },
    { test02_obsCubesInFrontYard, test01_trgPadsInFrontYard },

    // Plane 01
    { test00_obsCubesOnPlane01, test00_trgPadsOnPlane01 }
};
planeData *apPlane[] = {
    &aPlane[0],
    &aPlane[1],
    NULL
};


// 04. scene data containing planes
typedef struct {
    planeData **plane = apPlane;
} sampleSceneData;



// planeData array_planeInScene[] = {
//     { test00_obsCubesInBedRoom },
//     { test00_obsCubesOnPlane01 },
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
