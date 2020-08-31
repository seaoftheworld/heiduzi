#pragma once

#include <string>

class gpuProgram {
public:
    virtual ~gpuProgram() {}

    virtual void use() const = 0;
    virtual int getUniformLocation(const std::string &name) const = 0;
    virtual void setUniformMatrix4fv(int uniformId, const void *mat4f) const = 0;
    virtual void setClearColor(float *rgba) const = 0;

    static gpuProgram *create(const std::string& vsh, const std::string& fsh);
};

class verticesBuffer {
public:
    virtual ~verticesBuffer() {}

    virtual void bind() = 0;
    virtual void unbind() = 0;

    static verticesBuffer *create(const void *vertices, unsigned int size);
};

class indicesBuffer {
public:
    virtual ~indicesBuffer() {}

    virtual void bind() = 0;
    virtual void unbind() = 0;

    static indicesBuffer *create(const void *indices, unsigned int count);
};

class vertexArray {
public:
    virtual ~vertexArray() {}

    virtual void bind() = 0;
    virtual void unbind() = 0;

    static vertexArray *create();
};

class texture {
public:
    virtual ~texture() {}

    virtual unsigned int getWidth() const = 0;
    virtual unsigned int getHeight() const = 0;

    virtual void bind(unsigned char slot) const = 0;
};

class texture2D : public texture {
public:
    virtual ~texture2D() {}

    static texture2D *create(const std::string &path);
};

// class threedAPI {
// public:
//     virtual void init() = 0;
//     virtual void deinit() = 0;

//     // std::vector<verticesBuffer> vbo;
//     // std::vector<indicesBuffer>  ibo;
//     // std::vector<vertexArray>   vao;
// };

// void _dbg_3dDrawBox(const float *viewMat, const float *projMat, float (*vt)[3]);
// void _dbg_3dDrawBox(float (*vt)[3]);
// void _dbg_3dDrawBox(int mvpLoc, const float *mvpMat, float (*vt)[3]);
void _dbg_3dDrawBox(float *vt);

void _3d_printInfo();
void _3d_useTextureSlot(unsigned int textureUniformId, unsigned char slot);
void _3d_drawBuffers(
    verticesBuffer *vertices, 
    verticesBuffer *uvs,
    verticesBuffer *normals, indicesBuffer *ibo);
