#include "GroundRenderer.h"
#include "../graphics/Shader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

float vertices[] = {
    // Ground rectangle (two triangles forming a rectangle)
    -0.5f, -0.5f, 0.0f,
    0.5f, -0.5f, 0.0f,
    0.5f, 0.5f, 0.0f,

    -0.5f, -0.5f, 0.0f,
    0.5f, 0.5f, 0.0f,
    -0.5f, 0.5f, 0.0f};

GroundRenderer::GroundRenderer(Shader &shader) : shader(shader)
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    // shader.use();

    colorLoc = glGetUniformLocation(shader.getProgram(), "color");
    modelLoc = glGetUniformLocation(shader.getProgram(), "model");
    viewLoc = glGetUniformLocation(shader.getProgram(), "view");
    projLoc = glGetUniformLocation(shader.getProgram(), "projection");
}

GroundRenderer::~GroundRenderer()
{
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);

    // shader.use();

    colorLoc = glGetUniformLocation(shader.getProgram(), "color");
    modelLoc = glGetUniformLocation(shader.getProgram(), "model");
    viewLoc = glGetUniformLocation(shader.getProgram(), "view");
    projLoc = glGetUniformLocation(shader.getProgram(), "projection");
}

void GroundRenderer::draw(const glm::mat4 &view, const glm::mat4 &projection)
{
    shader.use();

    glUniform3f(colorLoc, 0.2f, 0.5f, 0.2f); // soft green

    // Create a model matrix (object's position and rotation)
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::scale(model, glm::vec3(100.0f, 1.0f, 100.0f)); // Scale the model
    model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}
