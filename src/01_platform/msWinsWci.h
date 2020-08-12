#pragma once

#include "wci.h"

class msWinsGameWindow : public gameWindow {
public:
    msWinsGameWindow() : m_Valid(0) { printf("  wwwwwwww\n"); }
    virtual ~msWinsGameWindow() { printf("  ~wwwwwwww\n"); }

    unsigned char init() override;
    void update() override;
    void deinit() override;

    void setCallback(const std::function<void()> &cb) override;
    void setValidation(unsigned char valid) override { m_Valid = valid; }
    unsigned char getValidation() override;

    double getCurrentTime() override;

private:
    unsigned char m_Valid;
};

class msWinsGameContext : public gameContext {
public:
    msWinsGameContext() { printf("  cccccccc\n"); }
    virtual ~msWinsGameContext() { printf("  ~cccccccc\n"); }

    unsigned char init(const void *pointer) override { return 1; }
    void swap() override;

    static void initFromWindow(void *pointer);
};

class msWinsGameInput : public gameInput {
public:
    msWinsGameInput() { printf("  iiiiiiiii\n"); }
    virtual ~msWinsGameInput() { printf("  ~iiiiiii\n"); }

    gameInputStatus read() override;
    // void getPointer(const void *handle) override;

private:
    // unsigned char inputByte = 0;
    inline unsigned char isAnyKeyPressed();
};



// To be called by gamePlatform::init() and gamePlatform::deinit()
unsigned char msWinsInitWCI(gameWindow **w, gameContext **c, gameInput **i);
void msWinsDeinitWCI(gameWindow **w, gameContext **c, gameInput **i);

    // Not used anymore, replaced by init()/deinit()
    // unsigned char msWinsCreateWCI(gameWindow **w, gameContext **c, gameInput **i);
    // void msWinsDeleteWCI(gameWindow **w, gameContext **c, gameInput **i);
