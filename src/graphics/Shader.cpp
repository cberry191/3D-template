#include "Shader.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/gtc/type_ptr.hpp>

Shader::Shader(const char *vertexPath, const char *fragmentPath)
{
    std::string vertexCode = readShaderFile(vertexPath);
    std::string fragmentCode = readShaderFile(fragmentPath);

    GLuint vertex = compileShader(vertexCode.c_str(), GL_VERTEX_SHADER);
    GLuint fragment = compileShader(fragmentCode.c_str(), GL_FRAGMENT_SHADER);

    programID = glCreateProgram();
    glAttachShader(programID, vertex);
    glAttachShader(programID, fragment);
    glLinkProgram(programID);

    glDeleteShader(vertex);
    glDeleteShader(fragment);

    GLint linked;
    glGetProgramiv(programID, GL_LINK_STATUS, &linked);
    if (!linked)
    {
        char infoLog[512];
        glGetProgramInfoLog(programID, 512, nullptr, infoLog);
        std::cerr << "Shader linking error: " << infoLog << "\n";
    }
}

std::string Shader::readShaderFile(const char *path)
{
    std::ifstream file(path, std::ios::in | std::ios::binary);
    if (!file)
    {
        std::cerr << "Error reading shader file: " << path << "\n";
        return "";
    }

    std::stringstream buffer;
    buffer << file.rdbuf();

    return buffer.str();
}

GLuint Shader::compileShader(const char *source, GLenum type)
{
    GLuint shader = glCreateShader(type);

    GLint length = (GLint)strlen(source);
    glShaderSource(shader, 1, &source, &length);
    glCompileShader(shader);

    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        std::cerr << "Shader compilation error: " << infoLog << "\n";
    }

    return shader;
}

Shader::~Shader()
{
    glDeleteProgram(programID);
}

void Shader::use()
{
    glUseProgram(programID);
}

GLuint Shader::getProgram()
{
    return programID;
}

void Shader::setMat4(const std::string &name, const glm::mat4 &mat) const
{
    glUniformMatrix4fv(glGetUniformLocation(programID, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
}

void Shader::setVec3(const std::string &name, const glm::vec3 &vec) const
{
    glUniform3f(glGetUniformLocation(programID, name.c_str()), vec.x, vec.y, vec.z);
}

void Shader::setFloat(const std::string &name, float value) const
{
    glUniform1f(glGetUniformLocation(programID, name.c_str()), value);
}

void Shader::setInt(const std::string &name, int value) const
{
    glUniform1i(glGetUniformLocation(programID, name.c_str()), value);
}

void Shader::setBool(const std::string &name, bool value) const
{
    glUniform1i(glGetUniformLocation(programID, name.c_str()), (int)value);
}