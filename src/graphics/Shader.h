#pragma once

#define GL_SILENCE_DEPRECATION
// #include <OpenGL/gl3.h>
#include <glad/glad.h>
#include <string>
#include <glm/glm.hpp>

class Shader
{
public:
    Shader(const char *vertexPath, const char *fragmentPath);
    ~Shader();

    void use();
    GLuint getProgram();

    void setMat4(const std::string &name, const glm::mat4 &mat) const;
    void setMat3(const std::string &, const glm::mat3 &);
    void setVec3(const std::string &name, const glm::vec3 &vec) const;
    void setFloat(const std::string &name, float value) const;
    void setInt(const std::string &name, int value) const;
    void setBool(const std::string &name, bool value) const;

private:
    GLuint programID;
    GLuint compileShader(const char *source, GLenum type);
    std::string readShaderFile(const char *path);
};
