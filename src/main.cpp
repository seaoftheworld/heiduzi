#include "01_platform/wci.h"  // Platform's WCI (window, context, input) and 'gamePlatform' singleton class defined here
#include "testLayer.h"        // Game layer

// #include <glm/glm.hpp>  // for testing, tobe removed
// #include "02_utils/objloader.hpp"
// #include "02_utils/vboindexer.hpp"

// Include standard headers
#include <stdio.h>

int main(void)
{
    gamePlatform &platform = gamePlatform::instance();  // singleton class' member returns 'Reference' !!!

    gameLayersManager layersManager;
    test_gameLayer    layer00;  

    // Init the WC: Window(s)/Context(s), and gl-Extentions
    if ( !platform.init() ) {  // Allocate instances  for 'm_Window', 'm_Context', and 'm_Input' pointers on heap-mem
        fprintf(stderr, "Failed to alloc-mem for the obj of: window, context, or input's subclass !\n"); getchar();
        return -1;
    }
    if ( !platform.getWindow()->init() ) {
        return -1;
    }
    // platform.m_Context->init();


    // The game begins by adding its layers into the 'layersMangaer'
    //
    // There're renderer(testRenderer) objects in this 'layer00'.
    // Shaders files are read, compiled, and gpu-program is generated with them (all at run time) when adding this layer.
    // Then vbos, ibos, and texture-buffers are generated (by loading .obj 3d-models and .png texture files).
    // in the next step of adding this layer (calling testRenderer's api).
    layersManager.pushLayer(&layer00);
    
    float lastTime_Frame = 0, lastTime_1s = 0;
    unsigned int frames = 0;
    while( platform.getWindow()->getValidation() ) {
        
        // 0. Update Timing by calling window-api
        float currentTime = (float)platform.getWindow()->getCurrentTime();
        float deltaTime = currentTime - lastTime_Frame;
        lastTime_Frame = currentTime;
        
        if ( currentTime - lastTime_1s >= 1.0f ){ // If more than 1s has passed
            // printf("  fps: %d/s.\n", frames);
            // printf("       %f ms/f.\n\n", 1000.0/(float)frames);

            // frames = 0;
            // lastTime_1s = currentTime;
        }

        // if (deltaTime > 0.5f) {
        //     printf("  __ may have been blocked, restart loop. delta: %f, \n", deltaTime);
            
        //     // clear input buffer
        //     //... ...

        //     // Clear the data
        //     //... ...
        //     lastTime_Frame = currentTime; 
        //     lastTime_1s = currentTime;
        //     frames = 0;
            
        //     continue;
        // }

        // Test for nonblocking_loadObj()
        /*
        // delta is about 0.0002 when nothing is called, delta increases to 0.002 when nonblocking_loadObj() is called 
        // (only open then close the .obj file, all other codes inside it commented off).
        static unsigned int repeat = 0, end_repeat = 0;
        static unsigned char result = 255;
        static unsigned char result2 = 255;

        static std::vector<glm::vec3> vertices;
        static std::vector<glm::vec2> uvs;
        static std::vector<glm::vec3> normals;

        static std::vector<glm::vec3> indexed_vertices;
        static std::vector<glm::vec2> indexed_uvs;
        static std::vector<glm::vec3> indexed_normals;
        static std::vector<unsigned short> indices;

        printf("  frame: %d, delta from previous frame: %f\n", frames, deltaTime);

        if (repeat > 4 && result != 0) {
            // Open/Close file time doesnt seem to be affected by file-size //

            // 200KB file, makes delta 0.0002 ==> 0.0007, the others also 0.0007 when tested at the same time
            // result = nonblocking_loadObj("assets/rooms/theYard/rb73_yForward_zUp.obj", vertices, uvs, normals);

            // 6MB file, makes delta 0.0002 ==> 0.002
            // result = nonblocking_loadObj("assets/rooms/theYard/btr80_obj3_only.obj", vertices, uvs, normals);

            // 864KB file, makes delta 0.0002 ==> 0.002
            result = nonblocking_loadObj("assets/rooms/theYard/low_poly_room_002.obj", vertices, uvs, normals);

            // nonblocking_indexVbo() tobe test...
            
            printf("  loadObj() returned: %d\n", result);
        }

        if (result2 != 0 && result == 0 ) {
            result2 = nonblocking_indexVbo(vertices, uvs, normals, 
                indices, indexed_vertices, indexed_uvs, indexed_normals);

            printf("  result2: %d\n", result2);
            if (result2 == 0) {
                end_repeat = repeat + 4;
            }
        }

        if (repeat == end_repeat && result == 0 && result2 == 0) {
            printf("end: %d, repeat: %d, quit! \n\n\n", end_repeat, repeat);
            break;
        }
        
        printf("\n\n");
        repeat++;
        //*/

            // 1. Input shall be processed at fixed interval with some design pattern in the future ???
            gameInputStatus input = gameInputStatus(0);
            input = platform.getInput()->read();

            // 2. Update the player's postion data (m of mvp matrixes in shader) accoridng to input, and
            //    Update the camera data (v of the mvp matrixes in shader) for the room according to player's position
            for (gameLayer *l : layersManager.m_Layers) {
                l->update(deltaTime, input);
            }

            // 3. Render the graphics by Updating the window (clear background before rendering) first
            platform.getWindow()->update();
            for (gameLayer *l : layersManager.m_Layers) {
                l->render();
            }
            platform.getContext()->swap();

        frames++;
    }
    
    // The game's logic ends when there're no layers
    layersManager.popLayer(&layer00);


    
    // platform.getContext()->deinit();
    if ( platform.getWindow() ) {
        platform.getWindow()->deinit();
    }
    platform.deinit();  // delete W/C/I instances allocated on heap mem by their pointers
    return 0;
}
