#pragma once

#include "3dAPI.h"

// Include GLEW for mordern open-gl extentions
// #include <GL/glew.h>

// Include GL3W for mordern open-gl extentions
#include <GL/gl3w.h>

class glGpuProgram : public gpuProgram {
public:
    glGpuProgram(const std::string &vsh, const std::string &fsh);
    virtual ~glGpuProgram();

    void use() const override;
    int getUniformLocation(const std::string &name) const override;
    void setUniformMatrix4fv(int uniformId, const void *mat4f) const override;
    void setClearColor(float *rgba) const override;

private:
    GLuint m_id = 0;
};

class glVerticesBuffer : public verticesBuffer {
public:
    glVerticesBuffer(const void *vertices, unsigned int size);  // 'size' means the size-in-byte
    virtual ~glVerticesBuffer();

    void bind() override;
    void unbind() override;

    inline GLuint getIdx() { return m_Idx; }
    inline GLuint getAttrIdx() { return m_AttrIdx; }

private:
    GLuint m_Idx;
    GLuint m_AttrIdx;
};

class glIndicesBuffer : public indicesBuffer {
public:
    glIndicesBuffer(const void *indices, unsigned int count);  // 'count' means the number_of_indices
    virtual ~glIndicesBuffer();

    void bind() override;
    void unbind() override;

    inline GLuint getIdx() { return m_Idx; }
    inline GLuint getCount() { return m_Count; }

private:
    GLuint m_Idx;
    GLuint m_Count;
};

class glVertexArray : public vertexArray {
public:
    glVertexArray();
    virtual ~glVertexArray();

    void bind() override;
    void unbind() override;

    inline GLuint getIdx() { return m_Idx; }

private:
    GLuint m_Idx;
};

class glTexture2D : public texture2D {
public:
    glTexture2D(const std::string &path);
    virtual ~glTexture2D();

    unsigned int getWidth() const override { return m_width; }
    unsigned int getHeight() const override { return m_height; }
    void bind(unsigned char slot) const override;

private:
    std::string m_path;
    unsigned int m_id;
    unsigned int m_width, m_height;
};


// ////////////////////////////////////////////
// void glDrawWithGeneratedBuffers(verticesBuffer **vbo, indicesBuffer *ibo);

// void _dbg_glDrawBox(const float *viewMat, const float *projMat, float (*vt)[3]);  // legacy gl implementation
// void _dbg_glDrawBox(float (*vt)[3]);  // awkward implementation relying on many testRenderer's methods to set uniform
// void _dbg_glDrawBox(int mvpLoc, const float *mvpMat, float (*vt)[3]);
void _dbg_glDrawBox(float *vt);

void _gl_printInfo();
extern void _gl_useTextureSlot(unsigned int textureUniformId, unsigned char slot);
extern void _gl_drawBuffers(
    verticesBuffer *vertices, 
    verticesBuffer *uvs, 
    verticesBuffer *normals, indicesBuffer *ibo);
