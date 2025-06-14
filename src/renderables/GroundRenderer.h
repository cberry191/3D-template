#pragma once
#include <GL/glew.h>
#define GL_SILENCE_DEPRECATION
// #include <OpenGL/gl3.h>
#include "../graphics/Shader.h"
#include <glm/glm.hpp>

class GroundRenderer
{
public:
    GroundRenderer(Shader &shader);
    ~GroundRenderer();

    void draw(const glm::mat4 &view, const glm::mat4 &projection);

private:
    GLuint VAO, VBO;
    Shader &shader;
    GLint modelLoc, viewLoc, projLoc, colorLoc;
};
