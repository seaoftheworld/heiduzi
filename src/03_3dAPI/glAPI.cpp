#include "glAPI.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>  // For printf()

void _gl_printInfo() {
    std::cout << "  OpenGL info:" << std::endl;
    std::cout << "     Vendor:  " << glGetString(GL_VENDOR) << std::endl;
    std::cout << "     Render:  " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "     Version: " << glGetString(GL_VERSION) << std::endl;
}

void _dbg_glDrawBox(float *vt) {
    // float testVert[] = {
    //     0.0f, 0.0f, 0.0f,
    //     0.0f, 1.0f, 1.0f
    // };
    // unsigned char testIndices[] = {0, 1};

    // glUniformMatrix4fv((GLint)mvpLoc, 1, GL_FALSE, (const GLfloat *)mvpMat);

    unsigned char indicesLineLoopTop[]    = {1, 3, 5, 7};  // 1357: the upper square
    unsigned char indicesLineLoopBottom[] = {0, 2, 4, 6};  // 0246: the lower square
    unsigned char indicesLine[] = {0, 1, 2, 3, 4, 5, 6, 7};  // 01, 23, 45, 67: 4 lines connecting the Upper and lower squares

    // Draw: topSquare + bottomSquare + 4_connecting lines, Without using vbos
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        glEnableVertexAttribArray(0);  // attribute-0 from dbg-program shader: vertexPosition
    
        glVertexAttribPointer(
            0,                  // attribute-0 from dbg-program shader: vertexPosition
            3, GL_FLOAT,        // 3-floats for each vertex
            GL_FALSE,           // normalized?
            0,                  // stride in bytes
            vt                  // array buffer (not offset without vbo)
        );

        glDrawElements(
            GL_LINE_LOOP,       // mode
            4,                  // count
            GL_UNSIGNED_BYTE,   // type
            indicesLineLoopTop  // element array buffer
        );

        glDrawElements(
            GL_LINE_LOOP,         // mode
            4,                    // count
            GL_UNSIGNED_BYTE,     // type
            indicesLineLoopBottom // element array buffer
        );

        glDrawElements(
            GL_LINES,           // mode
            8,                  // count
            GL_UNSIGNED_BYTE,   // type
            indicesLine         // element array buffer
        );
        // glDisableVertexAttribArray(0);

    // Draw a rectangle when 4 vertices (4 * 3-float) are passed in
    // unsigned char indicesLineLoopIdx[] = {0, 1, 2, 3};

    // glBindBuffer(GL_ARRAY_BUFFER, 0);
    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    //     glEnableVertexAttribArray(0);  // attribute-0 from dbg-program shader: vertexPosition
    
    //     glVertexAttribPointer(
    //         0,                  // attribute-0 from dbg-program shader: vertexPosition
    //         3, GL_FLOAT,        // 3-floats for each vertex
    //         GL_FALSE,           // normalized?
    //         0,                  // stride in bytes  // 12 in this case (3 * sizeof(float) = 12)
    //         vt                  // array buffer (not offset without vbo)
    //     );

    //     glDrawElements(
    //         GL_LINE_LOOP,       // mode
    //         4,                  // count
    //         GL_UNSIGNED_BYTE,   // type
    //         indicesLineLoopIdx  // element array buffer
    //     );
    //     // glDisableVertexAttribArray(0);
}

glGpuProgram::glGpuProgram(const std::string &vsh, const std::string &fsh) : m_id(0) {

    // Create the shaders
    GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

    // Read the Vertex Shader code from the file
    std::string VertexShaderCode;
    std::ifstream VertexShaderStream(vsh, std::ios::in);
    if(VertexShaderStream.is_open()){
        std::stringstream sstr;
        sstr << VertexShaderStream.rdbuf();
        VertexShaderCode = sstr.str();
        VertexShaderStream.close();
    }
    else {
        printf("Impossible to open %s. Right directory?\n", vsh.c_str());
        return;
    }

    // Read the Fragment Shader code from the file
    std::string FragmentShaderCode;
    std::ifstream FragmentShaderStream(fsh, std::ios::in);
    if(FragmentShaderStream.is_open()){
        std::stringstream sstr;
        sstr << FragmentShaderStream.rdbuf();
        FragmentShaderCode = sstr.str();
        FragmentShaderStream.close();
    }
    else {
        printf("Impossible to open %s. Right directory?\n", fsh.c_str());
        return;
    }

    // Compile Vertex Shader
    printf("Compiling shader : %s\n", vsh.c_str());
    char const * VertexSourcePointer = VertexShaderCode.c_str();
    glShaderSource(VertexShaderID, 1, &VertexSourcePointer , NULL);
    glCompileShader(VertexShaderID);
    // Check Vertex Shader
    {
        GLint Result = GL_FALSE; 
        int InfoLogLength = 0;
        
        glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
        if (!Result) {
            glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
            std::vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
            glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
            printf("vsh Error: %s\n", &VertexShaderErrorMessage[0]);
            return;
        }
    }

    // Compile Fragment Shader
    printf("Compiling shader : %s\n", fsh.c_str());
    char const * FragmentSourcePointer = FragmentShaderCode.c_str();
    glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer , NULL);
    glCompileShader(FragmentShaderID);
    // Check Fragment Shader
    {
        GLint Result = GL_FALSE; 
        int InfoLogLength = 0;
        
        glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
        if (!Result) {
            glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
            std::vector<char> FragmentShaderErrorMessage(InfoLogLength+1);
            glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
            printf("fsh Error: %s\n", &FragmentShaderErrorMessage[0]);
            return;
        }
    }

    // Link the program
    printf("Linking program\n");
    GLuint ProgramID = glCreateProgram();
    glAttachShader(ProgramID, VertexShaderID);
    glAttachShader(ProgramID, FragmentShaderID);
    glLinkProgram(ProgramID);
    // Check the program
    {
        GLint Result = GL_FALSE; 
        int InfoLogLength = 0;

        glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
        if (!Result){
            glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
            std::vector<char> ProgramErrorMessage(InfoLogLength+1);
            glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
            printf("%s\n", &ProgramErrorMessage[0]);
            return;
        }
    }

    m_id = ProgramID;
    printf("  __ programid: %d\n", m_id);
    
    glDetachShader(ProgramID, VertexShaderID);
    glDetachShader(ProgramID, FragmentShaderID);
    
    glDeleteShader(VertexShaderID);
    glDeleteShader(FragmentShaderID);
}

int glGpuProgram::getUniformLocation(const std::string &name) const {
    if (m_id) {
        GLint uniformId = glGetUniformLocation(m_id, name.c_str());
        return (int)uniformId;  // returns -1 (uniform invalid in shader), 0, 1, 2 ... ...
    }
    else
        return -2;              // returns -2 (program id invalid)
}

void glGpuProgram::use() const {
    if (m_id) {
        glUseProgram(m_id);

        // GLint attrLoc = glGetAttribLocation(m_id, "vertexPosition");
        // if (attrLoc >= 0) {
        //     printf("  __ \"vertexPosition\": %d\n", attrLoc);
        // }
    }
}

void glGpuProgram::setUniformMatrix4fv(int uniformId, const void *mat4f) const {
    glUniformMatrix4fv((GLint)uniformId, 1, GL_FALSE, (const GLfloat *)mat4f);
}

void glGpuProgram::setClearColor(float *rgba) const {
    if (rgba) {
        glClearColor(rgba[0], rgba[1], rgba[2], rgba[3]);
    }
}

glGpuProgram::~glGpuProgram() {
    if (m_id) {
        glDeleteProgram(m_id);
    }
}

// glVerticesBuffer
glVerticesBuffer::glVerticesBuffer(const void *vertices, unsigned int size) : m_Idx(0) {
    glGenBuffers(1, &m_Idx); 
    glBindBuffer(GL_ARRAY_BUFFER, m_Idx);
    // bind();
        glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    // unbind();

    printf("  gl vbo generated, idx: \'%d\'\n", (int)m_Idx);
}

glVerticesBuffer::~glVerticesBuffer() {
    glDeleteBuffers(1, &m_Idx);

    printf("  gl vbo deleted, idx: \'%d\'\n", (int)m_Idx);
}

void glVerticesBuffer::bind() {
    if (m_Idx) {
        glBindBuffer(GL_ARRAY_BUFFER, m_Idx);
    }

    // printf("  gl bind VBuffer idx: \'%d\'\n", (int)m_Idx);
}

void glVerticesBuffer::unbind() {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

// glIndicesBuffer
glIndicesBuffer::glIndicesBuffer(const void *indices, unsigned int count) : m_Idx(0), m_Count(count) {
    glGenBuffers(1, &m_Idx);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Idx);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned short), indices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    printf("  gl ibo generated, idx: \'%d\'\n", (int)m_Idx);
}

glIndicesBuffer::~glIndicesBuffer() {
    glDeleteBuffers(1, &m_Idx);

    printf("  gl ibo deleted, idx: \'%d\'\n", (int)m_Idx);
}

void glIndicesBuffer::bind() {
    if (m_Idx) {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Idx);
    }

    // printf("  gl bind IBuffer idx: \'%d\'\n", (int)m_Idx);
}

void glIndicesBuffer::unbind() {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

// glVertexArray
glVertexArray::glVertexArray() : m_Idx(0) {
    glGenVertexArrays(1, &m_Idx);
    printf("  gl vao generated, vao-idx: \'%d\'\n", (int)m_Idx);
}

glVertexArray::~glVertexArray() {
    glDeleteVertexArrays(1, &m_Idx);
    printf("  gl vao deleted, vao-idx: \'%d\'\n", (int)m_Idx);
}

void glVertexArray::bind() {
    if (m_Idx) {
        glBindVertexArray(m_Idx);
    }
}

void glVertexArray::unbind() {
    glBindVertexArray(0);
}

glTexture2D::glTexture2D(const std::string &path) : m_path(path), m_width(0), m_height(0), m_id(0) {
    
    int imgWidth, imgHeight, imgComp;

    // Maybe when uv is read from .obj format file, vertical-flip is already fixed ???
    // or maybe the loadOBJ() / indexVBO() func has fixed the vertical-flip issue ???
    stbi_set_flip_vertically_on_load(0);

    unsigned char *data = stbi_load(path.c_str(), &imgWidth, &imgHeight, &imgComp, 3);
    if (data) {
        m_width = imgWidth;
        m_height = imgHeight;

        // For elder versions of openGL (2.0 compatable ???)
        // glGenTextures(1, &textureID);
        // glBindTexture(GL_TEXTURE_2D, textureID);
        //     glPixelStorei(GL_UNPACK_ALIGNMENT,1);

        // unsigned int blockSize = (format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT) ? 8 : 16; 
        // unsigned int size = ((imgWidth + 3) / 4) * ((imgHeight + 3) / 4) * blockSize; 
        
        // glCompressedTexImage2D(GL_TEXTURE_2D, 0, format, width, height,  
        //         0, size, data); 
        glGenTextures(1, &m_id);
        glBindTexture(GL_TEXTURE_2D, m_id);
            // glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imgWidth, imgHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            // unsigned int blockSize = (format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT) ? 8 : 16; 
            // unsigned int size = ((imgWidth + 3) / 4) * ((imgHeight + 3) / 4) * blockSize; 
            // glCompressedTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, size, data); 

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


        // For latter versions of openGL (4.5 ???)
            // glCreateTextures(GL_TEXTURE_2D, 1, &m_id);                  // gen (maybe only the index, no mem allocated)
            // glTextureStorage2D(m_id, 1, GL_RGB8, imgWidth, imgHeight);  // allocate mem on gpu

            // glTextureParameteri(m_id, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            // glTextureParameteri(m_id, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            
            // glTextureSubImage2D(m_id, 0, 0, 0, imgWidth, imgHeight, GL_RGB, GL_UNSIGNED_BYTE, data);

        stbi_image_free(data);

        printf("  gl texture2d generated, id: \'%d\'\n", (int)m_id);
    }
    else {
    }
}

glTexture2D::~glTexture2D() {
    glDeleteTextures(1, &m_id);
    printf("  gl texture2d deleted, id: \'%d\'\n", (int)m_id);
}

void glTexture2D::bind(unsigned char slot) const {
    // For latter versions of openGL: works on nvd gl 4.6 drv, Not on intel 3.1 drv
    // glBindTextureUnit(slot, m_id);

    // For elder versions of openGL: works on both nvd gl 4.6 drv and intel 3.1 drv
    // openGL ES 2.0 ???
    GLenum para;
    switch (slot) {
        case 0:
        para = GL_TEXTURE0;
        break;
        
        case 1:
        para = GL_TEXTURE1;
        break;

        case 2:
        para = GL_TEXTURE2;
        break;
        
        case 3:
        para = GL_TEXTURE3;
        break;

        default:
        para = GL_TEXTURE4;
        break;
    }

    glActiveTexture(para); 
    glBindTexture(GL_TEXTURE_2D, m_id);
}



// extern GLuint TextureID;  // handle for the variable in shader, to be modified !!!
inline void _gl_useTextureSlot(unsigned int textureUniformId, unsigned char slot) {
    glUniform1i((GLint)textureUniformId, slot);
}

// void glDrawWithGeneratedBuffers(verticesBuffer **vbo, indicesBuffer *ibo) {
inline void _gl_drawBuffers(verticesBuffer *vertices, verticesBuffer *uvs, verticesBuffer *normals, indicesBuffer *ibo) {

        // float vert[] = {
        //     0.0f, 0.0f, 0.0f,
        //     0.0f, 1.0f, 1.0f
        // };
        // unsigned char indices[] = {
        //     0, 1
        // };

        // glBindBuffer(GL_ARRAY_BUFFER, 0);
        // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        // glEnableVertexAttribArray(0);  // attribute: vertexPosition

        //     glVertexAttribPointer(
        //         0,                  // attribute: vertexPosition
        //         3, GL_FLOAT,        // 3-floats for each vertex
        //         GL_FALSE,           // normalized?
        //         0,                  // stride
        //         vert                // array buffer (not offset without vbo)
        //     );

        //     glDrawElements(
        //         GL_LINES,           // mode
        //         2,                  // count
        //         GL_UNSIGNED_BYTE,   // type
        //         indices             // element array buffer
        //     );

    // glVerticesBuffer **glVbo = (glVerticesBuffer **)vbo;
    // glVerticesBuffer *glVertices = dynamic_cast<glVerticesBuffer *>(*vbo);  // ??? does it work in this way ???
    // glVerticesBuffer *glUVs = glVertices + 1;
    // glVerticesBuffer *glNormals = glVertices + 2;

    // ??? How to conver an array of pointers to baseclass to be pointers to subclass ???
    glVerticesBuffer *glVertices = dynamic_cast<glVerticesBuffer *>(vertices);
    glVerticesBuffer *glUVs = dynamic_cast<glVerticesBuffer *>(uvs);
    glVerticesBuffer *glNormals = dynamic_cast<glVerticesBuffer *>(normals);

    // 1rst attribute buffer : vertices
    // glBindBuffer(GL_ARRAY_BUFFER, ((*glVbo) + 0 )->getIdx() );
    // ((*glVbo) + 0)->bind();
    // glVertices->bind();
    glBindBuffer(GL_ARRAY_BUFFER, glVertices->getIdx());
    glEnableVertexAttribArray(0);
        glVertexAttribPointer(
            0,                  // attribute
            3,                  // size
            GL_FLOAT,           // type
            GL_FALSE,           // normalized?
            0,                  // stride
            (void*)0            // array buffer offset
        );

    // glVbo = dynamic_cast<glVerticesBuffer *>(*vbo + 1);
    // 2nd attribute buffer : UVs
    // glBindBuffer(GL_ARRAY_BUFFER, ((*glVbo) + 1 )->getIdx() );  // bind with wrong value
    // printf("  __ ((*glVbo) + 1 )->getIdx(): %d.\n", ((*glVbo) + 1)->getIdx());
    // ((*glVbo) + 1)->bind();                                     // crashes
    // glUVs->bind();
    glBindBuffer(GL_ARRAY_BUFFER, glUVs->getIdx() );
    glEnableVertexAttribArray(1);
        glVertexAttribPointer(
            1,                                // attribute
            2,                                // size
            GL_FLOAT,                         // type
            GL_FALSE,                         // normalized?
            0,                                // stride
            (void*)0                          // array buffer offset
        );

    // glVbo = dynamic_cast<glVerticesBuffer *>(*vbo + 2);
    // 3rd attribute buffer : normals
    // glBindBuffer(GL_ARRAY_BUFFER, ((*glVbo) + 2 )->getIdx() );
    // printf("  __ ((*glVbo) + 2 )->getIdx(): %d.\n", ((*glVbo) + 2)->getIdx());
    // ((*glVbo) + 2)->bind();
    // glNormals->bind();
    glBindBuffer(GL_ARRAY_BUFFER, glNormals->getIdx() );
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
    glIndicesBuffer *glIbo = dynamic_cast<glIndicesBuffer *>(ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, glIbo->getIdx() );
    // glIbo->bind();
        
        // Draw the triangles
        glDrawElements(
            // GL_POINTS,
            // GL_LINES,
            // GL_LINE_LOOP,
            GL_TRIANGLES,      // mode
            glIbo->getCount(), // count
            GL_UNSIGNED_SHORT, // type
            (void*)0           // element array buffer offset
        );

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
}

/*
static void dbg_glDrawLine(float (*from)[3], float (*to)[3], float (*color)[3]) {
    glColor3f( (*color)[0], (*color)[1], (*color)[2] );
    
    glBegin(GL_LINES);
        glVertex3f( (*from)[0], (*from)[1], (*from)[2] );
        glVertex3f( (*to)[0], (*to)[1], (*to)[2]);
    glEnd();
}

void _dbg_glDrawBox(const float *viewMat, const float *projMat, float (*vt)[3]) {
    // for(unsigned char i = 0; i < 8; i++) {
    //     printf("  __vt-%d: %f, %f, %f.\n", i, vt[i][0], vt[i][1], vt[i][2]);
    // }
    // printf("\n\n");

    glUseProgram(0);
        glMatrixMode(GL_MODELVIEW);
        glLoadMatrixf(viewMat);
        
        glMatrixMode(GL_PROJECTION);
        glLoadMatrixf(projMat);

    float color[] = {0.0f, 1.0f, 0.0f};
    
    // float start[] = {1.0f, 1.0f, 0.0f};
    // float end[] = {5.0f, 5.0f, 5.0f};
    // dbg_glDrawLine(&start, &end, &color);
    
    for (unsigned char i = 0; i < 8; i+=2) {// i: 0, 2, 4, 6 to draw top and bottom of the box
        // printf("  __aaa i: %d\n", i);
        dbg_glDrawLine(&vt[i], &vt[i + 1], &color);
    }

    for (unsigned char i = 0; i < 8; i++) {
        if (i % 2 == 0) {  // i: 0, 2, 4, 6
            // printf("  __bbb i: %d\n", i);
            dbg_glDrawLine(&vt[i], &vt[(i + 2) < 8 ? (i + 2) : (0)], &color);
        }
        else { // i: 1, 3, 5, 7
            // printf("  __ccc i: %d\n", i);
            dbg_glDrawLine(&vt[i], &vt[(i + 2) < 8 ? (i + 2) : (1)], &color);
        }
    }

}
//*/

// void _dbg_glDrawBox(float (*vt)[3]) {

//     // float testVert[] = {
//     //     0.0f, 0.0f, 0.0f,
//     //     0.0f, 1.0f, 1.0f
//     // };
//     // unsigned char testIndices[] = {0, 1};

//     float vert[8 * 3] = {0};
//     for (unsigned char i = 0; i < 8; i++) {
//         vert[ i * 3 ] = vt[i][0];
//         vert[ i * 3 + 1 ] = vt[i][1];
//         vert[ i * 3 + 2 ] = vt[i][2];
//         // printf("  __ i=%d: %f, %f, %f\n", (i * 3), vert[ i * 3 ], vert[ i * 3 + 1 ], vert[ i * 3 + 2 ]);
//     }
//     // printf("%s\n\n");
//     unsigned char indicesLineLoop[] = {  // 0246: the lower square, 1357: the upper square
//         0, 2, 4, 6, 
//         1, 3, 5, 7
//     };
//     unsigned char indicesLine[] = {      // 01, 23, 45, 67: 4 lines connecting the lower and upper square
//         0, 1, 
//         2, 3, 
//         4, 5, 
//         6, 7
//     };

//     // glUniformMatrix4fv((GLint)mvpLoc, 1, GL_FALSE, (const GLfloat *)mvpMat);

//     // Without vbos
//     glBindBuffer(GL_ARRAY_BUFFER, 0);
//     glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

//     glEnableVertexAttribArray(0);  // attribute: vertexPosition

//         glVertexAttribPointer(
//             0,                  // attribute: vertexPosition
//             3, GL_FLOAT,        // 3-floats for each vertex
//             GL_FALSE,           // normalized?
//             0,                  // stride
//             vert                // array buffer (not offset without vbo)
//         );

//         glDrawElements(
//             GL_LINE_LOOP,           // mode
//             4,                  // count
//             GL_UNSIGNED_BYTE,   // type
//             indicesLineLoop     // element array buffer
//         );

//         glDrawElements(
//             GL_LINE_LOOP,           // mode
//             4,                  // count
//             GL_UNSIGNED_BYTE,   // type
//             indicesLineLoop + 4     // element array buffer
//         );

//         glDrawElements(
//             GL_LINES,           // mode
//             8,                  // count
//             GL_UNSIGNED_BYTE,   // type
//             indicesLine     // element array buffer
//         );
// }

// void _dbg_glDrawBox(int mvpLoc, const float *mvpMat, float (*vt)[3]) {

//     glUniformMatrix4fv((GLint)mvpLoc, 1, GL_FALSE, (const GLfloat *)mvpMat);

//     float vert[8 * 3] = {0};
//     for (unsigned char i = 0; i < 8; i++) {
//         vert[ i * 3 + 0 ] = vt[i][0];
//         vert[ i * 3 + 1 ] = vt[i][1];
//         vert[ i * 3 + 2 ] = vt[i][2];
//         // printf("  __ i=%d: %f, %f, %f\n", (i * 3), vert[ i * 3 ], vert[ i * 3 + 1 ], vert[ i * 3 + 2 ]);
//     }
//     // printf("%s\n\n");
//     unsigned char indicesLineLoopTop[]    = {1, 3, 5, 7};  // 1357: the upper square
//     unsigned char indicesLineLoopBottom[] = {0, 2, 4, 6};  // 0246: the lower square
//     unsigned char indicesLine[] = {0, 1, 2, 3, 4, 5, 6, 7};  // 01, 23, 45, 67: 4 lines connecting the Upper and lower squares

//     // float testVert[] = {
//     //     0.0f, 0.0f, 0.0f,
//     //     0.0f, 1.0f, 1.0f
//     // };
//     // unsigned char testIndices[] = {0, 1};


//     // Without vbos
//     glBindBuffer(GL_ARRAY_BUFFER, 0);
//     glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

//     glEnableVertexAttribArray(0);  // attribute-0: vertexPosition

//         glVertexAttribPointer(
//             0,                  // attribute-0: vertexPosition
//             3, GL_FLOAT,        // 3-floats for each vertex
//             GL_FALSE,           // normalized?
//             0,                  // stride in bytes
//             vert                // array buffer (not offset without vbo)
//         );

//         glDrawElements(
//             GL_LINE_LOOP,       // mode
//             4,                  // count
//             GL_UNSIGNED_BYTE,   // type
//             indicesLineLoopTop  // element array buffer
//         );

//         glDrawElements(
//             GL_LINE_LOOP,         // mode
//             4,                    // count
//             GL_UNSIGNED_BYTE,     // type
//             indicesLineLoopBottom // element array buffer
//         );

//         glDrawElements(
//             GL_LINES,           // mode
//             8,                  // count
//             GL_UNSIGNED_BYTE,   // type
//             indicesLine         // element array buffer
//         );
//     // glDisableVertexAttribArray(0);


//     // With vbos
//     // GLuint vboIds[2];
//     // glGenBuffers(2, vboIds);

//     // glBindBuffer(GL_ARRAY_BUFFER, vboIds[0]);
//     // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIds[1]);
    
//     // glBufferData(GL_ARRAY_BUFFER, 12 * 8, vert, GL_STATIC_DRAW);
//     // glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned char) * 8, indicesLineLoop, GL_STATIC_DRAW);

//     // glEnableVertexAttribArray(0);  // attribute: vertexPosition

//     //     glVertexAttribPointer(
//     //         0,                  // attribute: vertexPosition
//     //         3, GL_FLOAT,        // 3-floats for each vertex
//     //         GL_FALSE,           // normalized?
//     //         0,                  // stride
//     //         (const void *)0     // array buffer (offset when vbo is used)
//     //     );

//     //     // Draw the triangles
//     //     glDrawElements(
//     //         GL_LINE_LOOP,       // mode
//     //         8,                  // count
//     //         GL_UNSIGNED_BYTE,   // type
//     //         0                   // element array buffer
//     //     );

//     // glDisableVertexAttribArray(0);

//     // glDeleteBuffers(2, vboIds);
// }
