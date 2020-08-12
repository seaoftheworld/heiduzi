#pragma once

#include <iostream>
#include <vector>

#include "01_platform/wci.h"  // Platform's WCI and gamePlatform defined here, to use 'gameInputStatus' for input data (just a byte), could be modified ???

class gameLayer {
public:
    gameLayer(const std::string &name = "__default_gameLayer") 
        : m_Name(name) {}
    virtual ~gameLayer() {}

    virtual void attach() = 0;
    virtual void dettach() = 0;
    virtual void update(float deltaTime, gameInputStatus input) = 0;
    virtual void render() = 0;

protected:
    std::string m_Name;
};

class gameLayersManager {
public:
    gameLayersManager();
    virtual ~gameLayersManager();

    void pushLayer(gameLayer *l);
    void popLayer(gameLayer *l);

    // std::vector<gameLayer *>::iterator begin() { return m_Layers.begin(); }
    // std::vector<gameLayer *>::iterator end() { return m_Layers.end(); }
// private:
    // std::vector<gameLayer *> m_Layers;  // a vector of pointer-to-gameLayer, could use 'delete' key word to deinit
    std::vector<gameLayer *> m_Layers;
    
private:
    unsigned char m_Idx;
};
