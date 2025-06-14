#pragma once

#include <GL/glew.h>
#define GL_SILENCE_DEPRECATION
// #include <OpenGL/gl3.h>
#include <string>

class Shader
{
public:
    Shader(const char *vertexPath, const char *fragmentPath);
    ~Shader();

    void use();
    GLuint getProgram();

private:
    GLuint programID;
    GLuint compileShader(const char *source, GLenum type);
    std::string readShaderFile(const char *path);
};
