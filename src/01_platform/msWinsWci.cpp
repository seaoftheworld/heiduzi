#include <iostream>
    // #include <vector>

// Include header file
#include "msWinsWci.h"

// Include GLEW for mordern open-gl extentions
// #include <GL/glew.h>
#include <GL/gl3w.h>

// Include GLFW for creating window and context
#include <GLFW/glfw3.h>

    // #include <common/matrices.hpp>

GLFWwindow *window;

    // extern GLuint programID, MatrixID, ViewMatrixID, ModelMatrixID, LightID; // , TextureID;

    // extern GLuint vertexbuffer0, uvbuffer0, normalbuffer0, elementbuffer0;
    // extern std::vector<unsigned short> indices0;

    // extern GLuint vertexbuffer1, uvbuffer1, normalbuffer1, elementbuffer1;
    // extern std::vector<unsigned short> indices1;

// For speed computation
double lastTime, textureLastTime;
unsigned int nbFrames = 0;

unsigned char msWinsGameWindow::init() {

    // 1. For the WC: Window(s)/Context(s) and Extentions, 
    // init GLFW (to be de-inited latter) and GLEW
    printf(" 0. Init GLFW (for ms-Wins and OpengGL)\n");
    if( !glfwInit() ) {
        fprintf( stderr, "Failed to initialize GLFW\n" );  getchar();
        return 0;
    }

    printf(" 1. Creating a Window, a Context, and the set the Input-mode... (with GLFW for OpengGL)\n");
    // glfwWindowHint(GLFW_SAMPLES, 4);

    // With the following 2 lines (which requrie opengl 4.5 / 3.3), 
    //     glBegin(GL_LINES) ... glEng() doesnt work
    // glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    // glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    
    // glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
    // glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Open a window and create its OpenGL context
    window = glfwCreateWindow( CTL_WIDTH, CTL_HEIGHT, "Tutorial 09 - VBO Indexing", NULL, NULL);
    if( window == NULL ){
        fprintf( stderr, "Failed to open GLFW window, requrie gl45 to use the glCreateTextures api !\n" ); getchar();
        glfwTerminate();
        return 0;
    }
    setValidation(1);
    msWinsGameContext::initFromWindow((void *)window);

    // Ensure we can capture the escape key being pressed below
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    // Hide the mouse and enable unlimited mouvement
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    
    // Set the mouse at the center of the screen
    glfwPollEvents();
    glfwSetCursorPos(window, CTL_WIDTH / 2, CTL_HEIGHT / 2);

    // printf(" 2. Init OpenGL extentions (with GL-E-W)...\n");
    // Initialize GLEW 
    // glewExperimental = true; // Needed for core profile
    // if (glewInit() != GLEW_OK) {
    //     fprintf(stderr, "Failed to initialize GLEW\n");  getchar();
    //     glfwTerminate();
    //     return 0;
    // }
    // printf("         OpenGL initilized with GL-E-W.\n");

    printf(" 2. Init OpenGL extentions (with GL-3-W)...\n");
    if ( gl3wInit() ) {
        fprintf(stderr, "Failed to initialize GLEW\n");  getchar();
        glfwTerminate();
        return 0;
    }
    printf("         OpenGL initilized with GL-3-W.\n");

        // Dark blue background
        glClearColor(0.0f, 0.0f, 0.4f, 1.0f);
        // Enable depth test
        glEnable(GL_DEPTH_TEST);
        // Accept fragment if it closer to the camera than the former one
        glDepthFunc(GL_LESS); 
        // Cull triangles which normal is not towards the camera
        // glEnable(GL_CULL_FACE);

    // lastTime = glfwGetTime();
    lastTime = getCurrentTime();
    textureLastTime = lastTime;
    return 1;
}

void msWinsGameWindow::update() {
        // // Measure speed
        // // double currentTime = glfwGetTime(); 
        // double currentTime = getCurrentTime();
        // nbFrames++;
        // if ( currentTime - lastTime >= 1.0 ){ // If last prinf() was more than 1sec ago
        //     // printf and reset
        //     // printf("%f ms/frame\n", 1000.0/double(nbFrames));
        //     printf("  fps: %d/s.\n", nbFrames);

        //     nbFrames = 0;
        //     // lastTime += 1.0;
        //     lastTime = currentTime;
        // }

    // Clear the screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // // static unsigned char textureUnitIdx = 0;
        // // glUniform1i(TextureID, ((textureUnitIdx + 1) > 2) ? (0) : (textureUnitIdx + 1) );

        // // Dynamically change the player's texture
        // if (currentTime - textureLastTime > 2.0f) {
        //     // static unsigned char textureUnitIdx = 1;
        //     // printf("  __textureUnitIdx 0: %d\n", textureUnitIdx);

        //     // Set "myTextureSampler" sampler to sample from Texture-Unit-0/1/2 with its uv coordinates from loading the model file
        //     // glUniform1i(TextureID, textureUnitIdx);  // uses texture-unit0 by default without specifying

        //     // textureUnitIdx = (textureUnitIdx >= 2) ? (0) : (++textureUnitIdx);
        //     // textureLastTime = currentTime;
        // }
}

inline unsigned char msWinsGameWindow::getValidation() {
    if (!m_Valid)
        return 0;

    if ( glfwGetKey(window, GLFW_KEY_ESCAPE ) == GLFW_PRESS || glfwWindowShouldClose(window) ) {
        setValidation(0);
        return 0;
    }
    else{
        return 1;
    }
}

inline double msWinsGameWindow::getCurrentTime() {
    return glfwGetTime();
}

void msWinsGameWindow::setCallback(const std::function<void()> &cb) {
    
}

void msWinsGameWindow::deinit() {
    printf("  win deinit called ...\n");
    glfwTerminate();
}

void msWinsGameContext::initFromWindow(void *pointer) {
    if (pointer) {
        GLFWwindow *p = static_cast<GLFWwindow *>(pointer);
        glfwMakeContextCurrent(p);
    }
}

void msWinsGameContext::swap() {
    glfwSwapBuffers(window);
    glfwPollEvents();
}

// void msWinsGameInput::getPointer(const void *handle) {
//     if (handle != NULL) {
//         // msWinsHandle = static_cast<const GLFWwindow *>(handle);
//     }
// }

// gameInputStatus msWinsGameInput::glfwRead() {
//     gameInputStatus valueFromGlfw = gameInputStatus(false, true, false, false, false, true, false);  // Backward with standby == 1
//     return valueFromGlfw;
// }

// #define BITMASK_UP     (1 << 7)
// #define BITMASK_DOWN   (1 << 6)
// #define BITMASK_LEFT   (1 << 5)
// #define BITMASK_RIGHT  (1 << 4)
// #define BITMASK_READY  (1 << 3)
// #define BITMASK_FIRE      (1 << 2)
// #define BITMASK_INVENTORY (1 << 1)

unsigned char msWinsGameInput::isAnyKeyPressed() {
    unsigned char inputByte = 0;

    inputByte = inputByte | ( (glfwGetKey(window, GLFW_KEY_W ) == GLFW_PRESS) ? (BITMASK_UP)   : (0) );
    inputByte = inputByte | ( (glfwGetKey(window, GLFW_KEY_S ) == GLFW_PRESS) ? (BITMASK_DOWN) : (0) );
    inputByte = inputByte | ( (glfwGetKey(window, GLFW_KEY_A ) == GLFW_PRESS) ? (BITMASK_LEFT) : (0) );
    inputByte = inputByte | ( (glfwGetKey(window, GLFW_KEY_D ) == GLFW_PRESS) ? (BITMASK_RIGHT): (0) );

    inputByte = inputByte | ( (glfwGetKey(window, GLFW_KEY_J ) == GLFW_PRESS) ? (BITMASK_READY)  : (0) );
    inputByte = inputByte | ( (glfwGetKey(window, GLFW_KEY_K ) == GLFW_PRESS) ? (BITMASK_FIRE)     : (0) );
    inputByte = inputByte | ( (glfwGetKey(window, GLFW_KEY_I ) == GLFW_PRESS) ? (BITMASK_INVENTORY): (0) );

    // if ( glfwGetKey(window, GLFW_KEY_W ) == GLFW_PRESS ) {
    //     inputByte |= BITMASK_UP;
    // }
    // if ( glfwGetKey(window, GLFW_KEY_S ) == GLFW_PRESS ) {
    //     inputByte |= BITMASK_DOWN;
    // }
    // if ( glfwGetKey(window, GLFW_KEY_A ) == GLFW_PRESS ) {
    //     inputByte |= BITMASK_LEFT;
    // }
    // if ( glfwGetKey(window, GLFW_KEY_D ) == GLFW_PRESS ) {
    //     inputByte |= BITMASK_RIGHT;
    // }

    // if ( glfwGetKey(window, GLFW_KEY_J ) == GLFW_PRESS ) {
    //     inputByte |= BITMASK_READY;
    // }
    // if ( glfwGetKey(window, GLFW_KEY_K ) == GLFW_PRESS ) {
    //     inputByte |= BITMASK_FIRE;
    // }
    // if ( glfwGetKey(window, GLFW_KEY_I ) == GLFW_PRESS ) {
    //     inputByte |= BITMASK_INVENTORY;
    // }

    return inputByte;
}

gameInputStatus msWinsGameInput::read() {
    unsigned char inputByte = isAnyKeyPressed();

    gameInputStatus inputStatus = gameInputStatus(0);
    inputStatus.m_byte = inputByte;

    // unsigned char inputByte = isAnyKeyPressed();

    // // Assign value for 'inputStatus' according to the result read from glfw api 
    // gameInputStatus inputStatus = gameInputStatus(false, false, false, false, false, false, false);

    // inputStatus.moveForward  = (inputByte & BITMASK_UP)    ? true : false;
    // inputStatus.moveBackward = (inputByte & BITMASK_DOWN)  ? true : false;
    // inputStatus.turnLeft     = (inputByte & BITMASK_LEFT)  ? true : false;
    // inputStatus.turnRight    = (inputByte & BITMASK_RIGHT) ? true : false;

    // inputStatus.trigger   = (inputByte & BITMASK_READY)   ? true : false;
    // inputStatus.fire      = (inputByte & BITMASK_FIRE)      ? true : false;
    // inputStatus.inventory = (inputByte & BITMASK_INVENTORY) ? true : false;

    // // if ( inputByte | BITMASK_UP ) {
    // //     inputStatus.moveForward = true;
    // // }
    // // if ( inputByte | BITMASK_DOWN ) {
    // //     inputStatus.moveForward = true;
    // // }
    // // if ( inputByte | BITMASK_LEFT ) {
    // //     inputStatus.turnLeft = true;
    // // }
    // // if ( inputByte | BITMASK_RIGHT ) {
    // //     inputStatus.turnRight = true;
    // // }

    // // if ( inputByte | BITMASK_READY ) {
    // //     inputStatus.trigger = true;
    // // }
    // // if ( inputByte | BITMASK_FIRE ) {
    // //     inputStatus.fire = true;
    // // }
    // // if ( inputByte | BITMASK_INVENTORY ) {
    // //     inputStatus.inventory = true;
    // // }

    return inputStatus;
}



unsigned char msWinsInitWCI(gameWindow **w, gameContext **c, gameInput **i) {

    *w = new msWinsGameWindow();
    if (*w == NULL) {
        return 0;
    }
    
    *c = new msWinsGameContext();
    if (*c == NULL) {
        return 0;
    }
    
    *i = new msWinsGameInput();
    if (*i == NULL) {
        return 0;
    }

    return 1;
}

void msWinsDeinitWCI(gameWindow **w, gameContext **c, gameInput **i) {
    // (*w)->deinit();
    // (*c)->deinit();
    // (*i)->deinit();

    delete *w; 
    delete *c; 
    delete *i;
}

    // unsigned char msWinsCreateWCI(gameWindow **w, gameContext **c, gameInput **i) {
    //     printf("  1111111\n");
    //     *w = new msWinsGameWindow();
    //     if (*w == NULL) {
    //         return 0;
    //     }
        
    //     *c = new msWinsGameContext();
    //     if (*c == NULL) {
    //         return 0;
    //     }
        
    //     *i = new msWinsGameInput();
    //     if (*i == NULL) {
    //         return 0;
    //     }
        
    //     printf("  2222222\n");
    //     return 1;
    // }
    // void msWinsDeleteWCI(gameWindow **w, gameContext **c, gameInput **i) {
    //     delete *w; 
    //     delete *c; 
    //     delete *i;
    // }

    // Use our shader
    // glUseProgram(programID);

        // // Compute the MVP matrix from keyboard and mouse input
        // computeMatricesFromInputs();

        // glm::mat4 ModelMatrix = glm::mat4(1.0);
        // glm::mat4 ViewMatrix = getViewMatrix();
        // glm::mat4 ProjectionMatrix = getProjectionMatrix();
        // glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
        // // glm::mat4 model(1.0); glm::vec3 axis(0.f, 0.f, 1.f);
        // // model = glm::rotate(model, (float)glfwGetTime(), axis);
        // // glm::mat4 MVP = ProjectionMatrix * ViewMatrix * model;

        // // Send our transformation to the currently bound shader, 
        // // in the "MVP" uniform
        // glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
        
        // glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
        // // glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &model[0][0]);
        
        // glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &ViewMatrix[0][0]);

    // glm::vec3 lightPos = glm::vec3(4,4,4);
    // glUniform3f(LightID, lightPos.x, lightPos.y, lightPos.z);

    /*
    glUniform1i(TextureID, textureUnitIdx);

    glm::mat4 playerModel(1.0); 
    playerModel = glm::translate(playerModel, vec3(2.f, 2.f, 0.f));
    
    glm::vec3 axis(0.f, 0.f, 1.f); 
    playerModel = glm::rotate(playerModel, (float)glfwGetTime(), axis);
    MVP = ProjectionMatrix * ViewMatrix * playerModel;

    glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &playerModel[0][0]);
    // Send our transformation to the currently bound shader, in the "MVP" uniform
    glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

    // 1rst attribute buffer : vertices
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer1);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(
            0,                  // attribute
            3,                  // size
            GL_FLOAT,           // type
            GL_FALSE,           // normalized?
            0,                  // stride
            (void*)0            // array buffer offset
        );

    // 2nd attribute buffer : UVs
    glBindBuffer(GL_ARRAY_BUFFER, uvbuffer1);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(
            1,                                // attribute
            2,                                // size
            GL_FLOAT,                         // type
            GL_FALSE,                         // normalized?
            0,                                // stride
            (void*)0                          // array buffer offset
        );

    // 3rd attribute buffer : normals
    glBindBuffer(GL_ARRAY_BUFFER, normalbuffer1);
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(
            2,                                // attribute
            3,                                // size
            GL_FLOAT,                         // type
            GL_FALSE,                         // normalized?
            0,                                // stride
            (void*)0                          // array buffer offset
        );

    // Index buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer1);

        // Draw the triangles
        glDrawElements(
            GL_TRIANGLES,      // mode
            indices1.size(),    // count
            GL_UNSIGNED_SHORT,   // type
            (void*)0           // element array buffer offset
        );

        // glDisableVertexAttribArray(0);
        // glDisableVertexAttribArray(1);
        // glDisableVertexAttribArray(2);
    //*/


    // Bind our texture in Texture Unit 0
    // glActiveTexture(GL_TEXTURE0);
    // glBindTexture(GL_TEXTURE_2D, pngTexture);

    // for (unsigned char i = 0; i < 3; i++) {
    //     glBindTextureUnit(i, pngTexture[i]);
    // }
