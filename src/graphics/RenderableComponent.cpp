#include "RenderableComponent.h"
#include "Shader.h"
#include "Mesh.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

RenderableComponent::RenderableComponent(Mesh *mesh, Shader *shader) : shader(shader), mesh(mesh)
{
    colorLoc = glGetUniformLocation(shader->getProgram(), "color");
    modelLoc = glGetUniformLocation(shader->getProgram(), "model");
    viewLoc = glGetUniformLocation(shader->getProgram(), "view");
    projLoc = glGetUniformLocation(shader->getProgram(), "projection");
}

void RenderableComponent::draw(glm::mat4 model, glm::mat4 view, glm::mat4 projection, glm::vec3 color)
{
    shader->use();

    glUniform3f(colorLoc, color.x, color.y, color.z);
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

    mesh->bind();
    mesh->draw();
}
