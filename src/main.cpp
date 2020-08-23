#include "01_platform/wci.h"  // Platform's WCI (window, context, input) and 'gamePlatform' singleton class defined here
#include "testLayer.h"        // Game layer

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
        frames++;
        if ( currentTime - lastTime_1s >= 1.0f ){ // If more than 1s has passed
            // printf("  fps: %d/s.\n", frames);
            // printf("       %f ms/f.\n\n", 1000.0/(float)frames);

            frames = 0;
            lastTime_1s = currentTime;
        }

        // 1. Input shall be processed at fixed interval with some design pattern in the future ???
        gameInputStatus input = platform.getInput()->read();

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
