#include "3dAPI.h"

// Include sub-class' header file to return pointer to a subclass for the baseclass' type
#include "glAPI.h"

gpuProgram *gpuProgram::create(const std::string &vsh, const std::string &fsh) {
    return new glGpuProgram(vsh, fsh);
}

verticesBuffer *verticesBuffer::create(const void *vertices, unsigned int size) {
    // return NULL;
    return new glVerticesBuffer(vertices, size);
}

indicesBuffer *indicesBuffer::create(const void *indices, unsigned int count) {
    // return NULL;
    return new glIndicesBuffer(indices, count);
}

vertexArray *vertexArray::create() {
    // return NULL;
    return new glVertexArray();
}

texture2D *texture2D::create(const std::string &path) {
    return new glTexture2D(path);
}

// void drawWithGeneratedBuffers(verticesBuffer **vbo, indicesBuffer *ibo) {
//     glDrawWithGeneratedBuffers(vbo, ibo);
// }
void _3d_printInfo() {
    _gl_printInfo();
}

// void _dbg_3dDrawBox(const float *viewMat, const float *projMat, float (*vt)[3]) {
//     _dbg_glDrawBox(viewMat, projMat, vt);
// }
// void _dbg_3dDrawBox(float (*vt)[3]) {
//     _dbg_glDrawBox(vt);
// }
// void _dbg_3dDrawBox(int mvpLoc, const float *mvpMat, float (*vt)[3]) {
//     _dbg_glDrawBox(mvpLoc, mvpMat, vt);
// }

void _dbg_3dDrawBox(float *vt) {
    _dbg_glDrawBox(vt);
}

void _3d_useTextureSlot(unsigned int textureUniformId, unsigned char slot) {
    _gl_useTextureSlot(textureUniformId, slot);
}

void _3d_drawBuffers(
    verticesBuffer *vertices, 
    verticesBuffer *uvs, 
    verticesBuffer *normals, indicesBuffer *ibo) {

    _gl_drawBuffers(vertices, uvs, normals, ibo);
}
