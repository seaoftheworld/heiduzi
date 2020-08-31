#pragma once

#include "03_3dAPI/3dAPI.h"       // to use verticesBuffer / indicesBuffer class
#include "actor.h"  // to use actor::position/yAngle to update the model-matrix for the object to be rendered

#include <string>
#include <iostream>

// These codes for drawing debugging-box are designed to be closely coupled 
// with the dbg-shaders in glTestShaders folder
void _dbgSetProgram();
void _dbgUnsetProgram();
void _dbgDrawBox(float *vt);

// These testRenderer codes designed to be closely coupled with the test-shaders
// in glTestShaders folder
class testRenderer {
public:
    typedef enum vBuffersId {
        vertices = 0, uvs, normals, indices, max
    } id;

    static void printInfo() {
        _3d_printInfo();
    }

    static void setClearColor(float *rgba);

    static void setTestGpuProgram();
    static void useTestGpuProgram();

    // static void getUniformLocation_MVP_Mat(const std::string &mvp);
    // static void getUniformLocation_Texture();
    // static void setMVP_Mat(const std::string &mvp, const void *p);
    // static void resetTestGpuProgram();
    static void getUniformLocation_MVP_Mat();
    static void getUniformLocation_Texture();
    static void setMVP_Mat(const void *p);
    
        // static void _dbg_setProgram();
        // static void _dbg_useProgram();
        // static  int _dbg_getUniformLocation_MVP_Mat_0();
        // static void _dbg_getUniformLocation_MVP_Mat();
        // static void _dbg_setMVP_Mat(const void *p);

    static void unsetProgram();


    void allocateVAO();             // by calling vertexArray::create()
    void freeVAO();
    void bindVAO() { m_vao->bind(); }
    void unbindVAO() { m_vao->unbind(); }

    // void allocateBuffers(room *p);  // by calling verticesBuffer::create()
    void allocateBuffers(const std::string &model, const std::string &texture);
    unsigned char nonblocking_allocateBuffers(const std::string &model, const std::string &texture);
    void freeBuffers();

    inline unsigned char getTextureSlot() const { return m_slot; }
    inline void setTextureSlot(unsigned char slot) { m_slot = slot; }
    inline void bindTextureToSlot() { 
        if (m_texture) { 
            m_texture->bind(m_slot);
        } 
    }

    void useTextureSlot();
    void drawBuffers();
    void drawBuffers(actor *act);

        // static void _dbg_drawBox(float (*vt)[3]);
        // static void _dbg_drawBox(int mvp_loc, float (*vt)[3]);

private:
    static gpuProgram *m_gpuProgram;
    static int m_mvp_UniformId;      
    static int m_texture_UniformId;  

        // static gpuProgram *m_dbgProgram;
        // static int m_dbg_mvp_UniformId;

    verticesBuffer *m_vBuffers[id::indices] = { NULL };  // for: vertices, uvs, normals (before 'indices')
    indicesBuffer *m_iBuffer = NULL;
    vertexArray *m_vao = NULL;

    texture2D *m_texture = NULL;
    unsigned char m_slot = 0;
};
