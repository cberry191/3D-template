#pragma once
#include "../graphics/Shader.h"
#include <glm/glm.hpp>

class PlayerRenderer
{
public:
    PlayerRenderer(Shader &shader);
    ~PlayerRenderer();
    void draw(const glm::vec3 &position, const glm::mat4 &view, const glm::mat4 &proj);

private:
    GLuint VAO, VBO;
    Shader &shader;
    GLint modelLoc, viewLoc, projLoc, colorLoc;
};
