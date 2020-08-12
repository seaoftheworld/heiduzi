#include "renderer.h"

// Include GLM to use glm::vec2/vec3 ... data types to represent data from 3d model file (like .obj), and call glm::translate()/rotate()
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

// To Read vertices/uv/normal/indices buffers-data from .obj 3d model file
#include "02_utils/objloader.hpp"
#include "02_utils/vboindexer.hpp"

    #include "02_utils/matrices.hpp"  // to call getViewMatrix(), getProjectionMatrix(), tobe removed...
    static glm::mat4 viewMatrix  = glm::mat4(1.0f);
    static glm::mat4 projMatrix = glm::mat4(1.0f);


// These codes for drawing debugging-box are designed to be closely coupled 
// with the dbg-shaders in glTestShaders folder
static gpuProgram *_dbgProgram = NULL;
static int _dbgStatus = -3;  // -1 returned from the api means 'the uniform is not an active uniform in the program'
                             // -2 returned means the program-id used to get uniform-id is invalid, So -3 is the init value.

void _dbgSetProgram() {
    _dbgProgram = gpuProgram::create("glTestShaders/_dbg_vsh.c", "glTestShaders/_dbg_fsh.c");

    if (!_dbgProgram) {
        _dbgStatus = -2;  // Failed to create dbg-program for gpu
    }
    else {
        _dbgStatus = _dbgProgram->getUniformLocation("MVP");  // -1: invalid; 0, 1, 2... valid unifor Id for "MVP"
    }
}

void _dbgUnsetProgram() {
    if (_dbgProgram) {
        delete _dbgProgram;
        _dbgStatus = -3;
    }
}

void _dbgDrawBox(float *vt) {
    if (_dbgStatus >= 0) {
        _dbgProgram->use();

        glm::mat4 modelMatrix = glm::mat4(1.0);
        viewMatrix = getViewMatrix();
        projMatrix = getProjectionMatrix();
        glm::mat4 mvpMat = projMatrix * viewMatrix * modelMatrix;

        _dbgProgram->setUniformMatrix4fv(_dbgStatus, &mvpMat[0][0]);

        _dbg_3dDrawBox(vt);
    }
}

// These testRenderer codes designed to be closely coupled with the test-shaders
// in glTestShaders folder

// Static members of 'testRenderer' class initialized here
gpuProgram *testRenderer::m_gpuProgram = NULL;
int testRenderer::m_mvp_UniformId     = -3;  // -1 returned from the api means 'the uniform is not an active uniform in the program'
int testRenderer::m_texture_UniformId = -3;  // -2 returned means the program-id is invalid, So -3 is the init value.

void testRenderer::setTestGpuProgram() {
    // m_gpuProgram = gpuProgram::create("StandardShading.vertexshader", "StandardShading.fragmentshader");
    m_gpuProgram = gpuProgram::create("glTestShaders/vsh.txt", "glTestShaders/fsh.txt");
}

void testRenderer::useTestGpuProgram() {
    if (m_gpuProgram) {
        m_gpuProgram->use();
    }
}

void testRenderer::unsetProgram() {
    if (m_gpuProgram) {
        delete m_gpuProgram;
    }
}

// void testRenderer::getUniformLocation_MVP_Mat(const std::string &mvp) {
//     if (!m_gpuProgram)
//         return;

//     if (mvp == "MVP") {
//         m_mvp_UniformId = m_gpuProgram->getUniformLocation(mvp);
//     }
//     else if (mvp == "M") {  // returns -1, uniform not valid
//         m_m_UniformId = m_gpuProgram->getUniformLocation(mvp);
//     }
//     else if (mvp == "V") {  // returns -1, uniform not valid
//         m_v_UniformId = m_gpuProgram->getUniformLocation(mvp);
        
//         printf("  __ mvp, m, v ids: %d, %d, %d\n", m_mvp_UniformId,
//             m_m_UniformId, m_v_UniformId);
//     }
// }
void testRenderer::getUniformLocation_MVP_Mat() {
    if (!m_gpuProgram) {
        m_mvp_UniformId = -2;
    }
    else {
        m_mvp_UniformId = m_gpuProgram->getUniformLocation("MVP");
    }

}

void testRenderer::getUniformLocation_Texture() {
    if (!m_gpuProgram) {
        m_texture_UniformId = -2;
    }
    else {
        m_texture_UniformId = m_gpuProgram->getUniformLocation("myTextureSampler");
    }
}

// void testRenderer::setMVP_Mat(const std::string &mvp, const void *p) {
//     if (mvp == "MVP" && m_mvp_UniformId >= 0) {
//         m_gpuProgram->setUniformMatrix4fv(m_mvp_UniformId, p);
//     }
//     else if (mvp == "M" && m_m_UniformId >= 0) {  // uniform not valid
//         m_gpuProgram->setUniformMatrix4fv(m_m_UniformId, p);
//     }
//     else if (mvp == "V" && m_v_UniformId >= 0) {  // uniform not valid
//         m_gpuProgram->setUniformMatrix4fv(m_v_UniformId, p);
//     }
// }
void testRenderer::setMVP_Mat(const void *p) {
    if (m_mvp_UniformId >= 0) {
        m_gpuProgram->setUniformMatrix4fv(m_mvp_UniformId, p);
    }
}

void testRenderer::allocateVAO() {
    m_vao = vertexArray::create();
}

void testRenderer::freeVAO() {
    delete m_vao;
}

void testRenderer::allocateBuffers(const std::string &model, const std::string &texture) {
    // Read data from .obj files
    std::vector<glm::vec3> indexed_vertices;
    std::vector<glm::vec2> indexed_uvs;
    std::vector<glm::vec3> indexed_normals;
    std::vector<unsigned short> indices;

    std::vector<glm::vec3> vertices;
    std::vector<glm::vec2> uvs;
    std::vector<glm::vec3> normals;
    bool res = loadOBJ(model.c_str(), vertices, uvs, normals);

    if (res) {
        indexVBO(vertices, uvs, normals, 
            indices, indexed_vertices, indexed_uvs, indexed_normals);

        const void *bufferPointer[id::max] = {
            (const void *) &indexed_vertices[0],
            (const void *) &indexed_uvs[0],
            (const void *) &indexed_normals[0],
            (const void *) &indices[0]
        };
        unsigned int bufferSize[id::max] = {
            indexed_vertices.size() * sizeof(glm::vec3),
            indexed_uvs.size() * sizeof(glm::vec2),
            indexed_normals.size() * sizeof(glm::vec3),
            indices.size()
        };
        for (unsigned int i = id::vertices; i < id::max; i++) {
            if (i != id::indices) {
                m_vBuffers[i] = verticesBuffer::create(bufferPointer[i], bufferSize[i]);
            }
            else {
                m_iBuffer = indicesBuffer::create(bufferPointer[i], bufferSize[i]);
            }
        }

        m_texture = texture2D::create(texture.c_str());
    }
}

void testRenderer::freeBuffers() {
    delete m_texture;

    for (unsigned int i = id::vertices; i < id::max; i++) {
        if (i != id::indices) {
            delete m_vBuffers[i];
        }
        else {
            delete m_iBuffer;
        }
    }
}

void testRenderer::useTextureSlot() {
    // m_texture_UniformId = TextureID;
    _3d_useTextureSlot(m_texture_UniformId, m_slot);
}

void testRenderer::drawBuffers() {
    glm::mat4 modelMatrix = glm::mat4(1.0);  // Model-mat for the room, which does not change mostly
    viewMatrix = getViewMatrix();            // V-mat for the room, which should have been updated
    projMatrix = getProjectionMatrix();      // P-mat, which should have not changed after updated

    glm::mat4 MVP = projMatrix * viewMatrix * modelMatrix;  // could also be done in the shader
    setMVP_Mat(&MVP[0][0]);

    // Implemented in 3dAPI.cpp
    _3d_drawBuffers(m_vBuffers[id::vertices], m_vBuffers[id::uvs], m_vBuffers[id::normals], m_iBuffer);
}

void testRenderer::drawBuffers(actor *act) {
    if (act) {
        glm::mat4 modelMat = glm::translate(glm::mat4(1.0), act->position);
        modelMat = glm::rotate(modelMat, act->yAngle, glm::vec3(0.f, 0.f, 1.f));

        viewMatrix = getViewMatrix();        // V-mat for the player, which should have not been changed even after updated
        projMatrix = getProjectionMatrix();  // P-mat for player, same as above

        glm::mat4 MVP = projMatrix * viewMatrix * modelMat;  // could also be done in the shader
        setMVP_Mat(&MVP[0][0]);
    }

    // Implemented in 3dAPI.cpp
    _3d_drawBuffers(m_vBuffers[id::vertices], m_vBuffers[id::uvs], m_vBuffers[id::normals], m_iBuffer);
}

// gpuProgram *testRenderer::m_dbgProgram = NULL;
// int testRenderer::m_dbg_mvp_UniformId = -3;

// void testRenderer::_dbg_setProgram() {
//     // For debugging
//     m_dbgProgram = gpuProgram::create("shaders/_dbg_vsh.c", "shaders/_dbg_fsh.c");
// }

// void testRenderer::_dbg_useProgram() {
//     if (m_dbgProgram) {
//         m_dbgProgram->use();
//     }
// }

// int testRenderer::_dbg_getUniformLocation_MVP_Mat_0() {
//     if (!m_dbgProgram)
//         return -2;

//     // Error !!! forgot to return the value, wrong value returned !!!
//     // Causing rendering failure.
//     int loc = m_dbgProgram->getUniformLocation("MVP");  // returns 53

//     return loc;
// }

// void testRenderer::_dbg_getUniformLocation_MVP_Mat() {
//     if (!m_dbgProgram) {
//         return;
//     }

//     m_dbg_mvp_UniformId = m_dbgProgram->getUniformLocation("MVP");
//     printf("  __ m_dbg_mvp_UniformId: %d\n", m_dbg_mvp_UniformId);
// }

// void testRenderer::_dbg_setMVP_Mat(const void *p) {
//     if (m_dbg_mvp_UniformId >= 0) {
//         m_dbgProgram->setUniformMatrix4fv(m_dbg_mvp_UniformId, p);
//     }
// }

// void testRenderer::_dbg_drawBox(float (*vt)[3]) {
//     viewMatrix = getViewMatrix();            // V-mat for the room, which should have been updated
//     projMatrix = getProjectionMatrix();      // P-mat, which should have not changed after updated

//     _dbg_3dDrawBox(&viewMatrix[0][0], &projMatrix[0][0], vt);
// }

// void testRenderer::_dbg_drawBox(int mvp_loc, float (*vt)[3]) {
//     glm::mat4 modelMatrix = glm::mat4(1.0);
//     viewMatrix = getViewMatrix();          
//     projMatrix = getProjectionMatrix();    

//     // glm::mat4 mvpMat = projMatrix * viewMatrix * modelMatrix;
//     glm::mat4 mvpMat = projMatrix * viewMatrix * modelMatrix;
//     _dbg_setMVP_Mat(&mvpMat[0][0]);

//     // Implemented in 3dAPI.cpp
//     // _dbg_3dDrawBox(mvp_loc, &mvpMat[0][0], vt);
//     _dbg_3dDrawBox(vt);
// }

// void testRenderer::_dbg_drawBox(float (*vt)[3]) {
//     glm::mat4 modelMatrix = glm::mat4(1.0);
//     viewMatrix = getViewMatrix();          
//     projMatrix = getProjectionMatrix();    

//     glm::mat4 mvpMat = projMatrix * viewMatrix * modelMatrix;
//     _dbg_setMVP_Mat(&mvpMat[0][0]);

//     _dbg_3dDrawBox(vt);
// }