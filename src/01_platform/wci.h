#pragma once

/*
#ifdef __CONTROLS_CPP_CODE__
    void computeMatricesFromInputs();
    glm::mat4 getViewMatrix();
    glm::mat4 getProjectionMatrix();
#endif
//*/

// In this design, window/context/input are closely coupled with each other

#include <functional>

#define CTL_WIDTH (800)
#define CTL_HEIGHT (400)

// 1. For the game's window: 'gameWindow' class
class gameWindow {
public:
    gameWindow() {}
    virtual ~gameWindow() {}

    virtual unsigned char init() = 0;
    virtual void update() = 0;
    virtual void deinit() = 0;

    virtual void setCallback(const std::function<void()> &cb) = 0;
    virtual void setValidation(unsigned char valid) = 0;
    virtual unsigned char getValidation() = 0;
    // static gameWindow *create();

    virtual double getCurrentTime() = 0;
};

// 2. For the game's context: 'gameContext' class
class gameContext {
public:
    gameContext() {}
    virtual ~gameContext() {}

    virtual unsigned char init(const void *pointer) = 0;
    virtual void swap() = 0;
    // static gameContext *create();
};

// 3. For the game's input: 'gameInput' class
#define BITMASK_UP     (1 << 7)
#define BITMASK_DOWN   (1 << 6)
#define BITMASK_LEFT   (1 << 5)
#define BITMASK_RIGHT  (1 << 4)
#define BITMASK_TRIGGER   (1 << 3)
#define BITMASK_FIRE      (1 << 2)
#define BITMASK_INVENTORY (1 << 1)

struct gameInputStatus {  // game specific status
    // bool moveForward, moveBackward, turnLeft, turnRight;
    // bool trigger, fire, inventory;

    // gameInputStatus(bool up, bool down, bool left, bool right, bool a, bool b, bool c) : 
    //     moveForward(up), moveBackward(down), turnLeft(left), turnRight(right),
    //     trigger(a), fire(b), inventory(c) {
    // }

    gameInputStatus(unsigned char byte = 0) : m_byte(byte) {}

    unsigned char m_byte;
};
    
class gameInput {
public:
    gameInput() {}
    virtual ~gameInput() {}

    virtual gameInputStatus read() = 0;
    // virtual void getPointer(const void *handle) = 0;
    // static gameInput *create();
};

class gamePlatform {
public:
    static gamePlatform &instance() {
        static gamePlatform platform;  // Singleton class object, the Only one.
        return platform;
    }

    // Not used anymore, repalced by the following api
        // unsigned char createWindowContextInput(gameWindow *w, gameContext *c, gameInput *i);
        // void deleteWindowContextInput(gameWindow *w, gameContext *c, gameInput *i);
        // unsigned char createWindowContextInput();  
        // void deleteWindowContextInput();
    unsigned char init();  // create Window, Context, and Input, and init them
    void deinit();         // deinit wci, and delete them.

    gameWindow  *getWindow() { return m_Window; }
    gameContext *getContext() { return m_Context; }
    gameInput   *getInput() { return m_Input; }

private:
    gamePlatform() {}
    
    gameWindow  *m_Window = NULL;
    gameContext *m_Context = NULL;
    gameInput   *m_Input = NULL;
};

// class gamePlatform_ {
// public:
//     gamePlatform_(const gamePlatform_ &) = delete;

//     static gamePlatform_ &instance() {  // Returns a 'reference' !!!
//         static gamePlatform_ p;
//         return p; 
//     }

//     gameWindow *getWindow() { return m_Window; }
//     gameContext *getContext() { return m_Context; }
//     gameInput *getInput() { return m_Input; }

//     unsigned char init();  // create Window, Context, and Input, and init them
//     void deinit();         // deinit wci, and delete them.

// private:
//     gamePlatform_() {}
//     // virtual ~gamePlatform_() {}

//     gameWindow  *m_Window = NULL;
//     gameContext *m_Context = NULL;
//     gameInput   *m_Input = NULL;

// };
