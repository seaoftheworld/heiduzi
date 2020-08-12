/*
// Include GLFW
#include <GLFW/glfw3.h>
extern GLFWwindow* window; // The "extern" keyword here is to access the variable "window" declared in tutorialXXX.cpp. This is a hack to keep the tutorials simple. Please avoid this.

// Include GLM, then Include the controls.hpp, since control.hpp has 'glm::mat4' declaration
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
    #include <glm/gtc/type_ptr.hpp>
using namespace glm;
//*/

// #define __CONTROLS_CPP_CODE__  // Including the part with 'glm::mat4' declaration 
#include "wci.h"

// Including header-files for sub-classes 
#include "msWinsWci.h"

// For the game's window/context: 'gameWindow' and 'gameContext' class,
// this could support msWindows and X11 windows, not android.
// gameWindow *gameWindow::create() {
//     return new msWinsGameWindow();  // Return an obj for the subclass according to the platform/configuration
// }
// gameContext *gameContext::create() {
//     return new msWinsGameContext();
// }

// // For the game's input: 'gameInput' class
//     // class testGameInput: public gameInput {
//     // public:
//     //     gameInputStatus read() override { return s; }
//     // private:
//     //     gameInputStatus s = {true, false, false, false, false, true, false};  // Forward with standby == 1
//     // };
// gameInput *gameInput::create() {
//     return new msWinsGameInput();
//     // return new testGameInput();
// }

// To be replaced by a singleton class
// gamePlatform platform;

unsigned char gamePlatform::init() {
    // call Platform-Specific implementation ...
    return msWinsInitWCI(&m_Window, &m_Context, &m_Input);
    // return 0;
}

void gamePlatform::deinit() {
    // call Platform-Specific implementation ...
    msWinsDeinitWCI(&m_Window, &m_Context, &m_Input);
}

    // unsigned char gamePlatform::createWindowContextInput() {
    //     // call Platform-Specific implementation ...
    //     return msWinsCreateWCI(&m_Window, &m_Context, &m_Input);
    // }
    // void gamePlatform::deleteWindowContextInput() {
    //     // This could be platform-independent using c++ keyword 'delete'
    //     msWinsDeleteWCI(&m_Window, &m_Context, &m_Input);
    // }


/*
glm::mat4 ViewMatrix;
glm::mat4 ProjectionMatrix;

glm::mat4 getViewMatrix(){
    return ViewMatrix;
}
glm::mat4 getProjectionMatrix(){
    return ProjectionMatrix;
}


// Initial position : on +Z
glm::vec3 position = glm::vec3(2.f, -4.f, 1.f);
glm::vec3 camDirection(0.f, 1.f, 0.f);
glm::vec3 camUp(0.f, 0.f, 1.f);

// glm::vec3 cameraPosition(2.f, -4.f, 1.f);
// glm::vec3 cameraDirection(0.f, 1.f, 0.f);
// glm::vec3 cameraUp(0.f, 0.f, 1.f);

// Initial horizontal angle : toward -Z
// float horizontalAngle = 3.14f;
float horizontalAngle = 0.f;
// Initial vertical angle : none
float verticalAngle = 0.0f;

// Initial Field of View
float initialFoV = 45.0f;

float speed = 3.0f; // 3 units / second
float mouseSpeed = 0.005f;



float prev_horizontalAngle = 0, prev_veticalAngle = 0;

static void printGlmMat4(const char* prefix, glm::mat4 m) {

    unsigned char i, j;
    const float *p = (const float*)glm::value_ptr(m);

    for (i = 0; i < 4; i++) {
        printf("%s[%d, 0~3]: ", prefix, i);
        for (j = 0; j < 4; j++) {
            printf("%f, ", p[i * 4 + j]);
        }
        printf("\n");
    }
    printf("\n\n");
}

static void printfGlmVec4(const char *prefix, glm::vec4 v4) {
    const float* p = glm::value_ptr(v4);
    printf("%s: ", prefix);
    for (unsigned char i = 0; i < 4; i++) {
        printf("%f, ", p[i]);
    }
    printf("\n");
}

void computeMatricesFromInputs(){

    // glfwGetTime is called only once, the first time this function is called
    static double lastTime = glfwGetTime();

    // Compute time difference between current and last frame
    double currentTime = glfwGetTime();
    float deltaTime = float(currentTime - lastTime);

    // Get mouse position
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);

    // Reset mouse position for next frame
    glfwSetCursorPos(window, CTL_WIDTH / 2, CTL_HEIGHT / 2);

    // Compute new orientation
    // horizontalAngle += mouseSpeed * float(CTL_WIDTH / 2 - xpos );
    // verticalAngle   += mouseSpeed * float( CTL_HEIGHT / 2 - ypos );
    horizontalAngle += mouseSpeed * (float)(xpos - CTL_WIDTH / 2);  // moveLeft --, moveRight ++
    verticalAngle += mouseSpeed * (float)(CTL_HEIGHT / 2 - ypos);   // moveUp ++, moveDown --

    // Direction : Spherical coordinates to Cartesian coordinates conversion
    // glm::vec3 direction(
    //  cos(verticalAngle) * sin(horizontalAngle), 
    //  sin(verticalAngle),
    //  cos(verticalAngle) * cos(horizontalAngle)
    // );
    // glm::vec3 direction(
    camDirection = glm::vec3(
        cos(verticalAngle) * sin(horizontalAngle), 
        cos(verticalAngle) * cos(horizontalAngle),
        sin(verticalAngle)
    );
    
    // Right vector
    // glm::vec3 right = glm::vec3(
    //  sin(horizontalAngle - 3.14f/2.0f), 
    //  0,
    //  cos(horizontalAngle - 3.14f/2.0f)
    // );
    glm::vec3 camRight = glm::vec3(
        sin(3.14f/2.0f - horizontalAngle), 
        cos(-3.14f/2.0f - horizontalAngle),
        0
    );

    // Up vector
    // glm::vec3 up = glm::cross( right, camDirection );
    camUp = glm::cross( camRight, camDirection );

    if (horizontalAngle != prev_horizontalAngle || verticalAngle != prev_veticalAngle) {
        // printf("  hAng, vAng: %f, %f\n", horizontalAngle, verticalAngle);
        // printfGlmVec4("dirc: ", glm::vec4(camDirection, 0.f));
        // printfGlmVec4("righ: ", glm::vec4(camRight, 0.f));
        // printf("\n");

        // printf("  x,y: %f, %f\n", xpos, ypos);
        // printf("\n");

        prev_horizontalAngle = horizontalAngle;
        prev_veticalAngle = verticalAngle;
    }

    // Move forward
    if (glfwGetKey( window, GLFW_KEY_UP ) == GLFW_PRESS){
        position += camDirection * deltaTime * speed;
    }
    // Move backward
    if (glfwGetKey( window, GLFW_KEY_DOWN ) == GLFW_PRESS){
        position -= camDirection * deltaTime * speed;
    }
    // Strafe right
    if (glfwGetKey( window, GLFW_KEY_RIGHT ) == GLFW_PRESS){
        position += camRight * deltaTime * speed;
    }
    // Strafe left
    if (glfwGetKey( window, GLFW_KEY_LEFT ) == GLFW_PRESS){
        position -= camRight * deltaTime * speed;
    }

    if ( glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        // float speed = .01f;
        position.y += speed * deltaTime;
    }
    if ( glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        // float speed = .01f;
        position.y -= speed * deltaTime;
    }
    if ( glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        // float speed = .01f;
        position.x -= speed * deltaTime;
    }
    if ( glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        // float speed = .01f;
        position.x += speed * deltaTime;
    }
    if ( glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
        // float speed = .01f;
        position.z += speed * deltaTime;
    }
    if ( glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS ) {
        // float speed = .01f;
        position.z -= speed * deltaTime;
    }

    float FoV = initialFoV;// - 5 * glfwGetMouseWheel(); // Now GLFW 3 requires setting up a callback for this. It's a bit too complicated for this beginner's tutorial, so it's disabled instead.

    // Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
    ProjectionMatrix = glm::perspective(glm::radians(FoV), (float)CTL_WIDTH / CTL_HEIGHT, 0.1f, 100.0f);
    // Camera matrix
    ViewMatrix       = glm::lookAt(
                                position,           // Camera is here
                                position + camDirection, // and looks here : at the same position, plus "camDirection"
                                camUp                  // Head is up (set to 0,-1,0 to look upside-down)
                           );

    // For the next frame, the "last time" will be "now"
    lastTime = currentTime;
}
//*/


