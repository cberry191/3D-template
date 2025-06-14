// #include "../graphics/Shader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
// #include <glad/glad.h>
#include <iostream>
#include "PlayerRenderer.h"

float cubeVertices[] = {
    // Cube vertices (36 total, 12 triangles)
    -0.5f, -0.5f, 0.5f, // front face
    0.5f, -0.5f, 0.5f,
    0.5f, 0.5f, 0.5f,
    0.5f, 0.5f, 0.5f,
    -0.5f, 0.5f, 0.5f,
    -0.5f, -0.5f, 0.5f,

    -0.5f, -0.5f, -0.5f, // back face
    -0.5f, 0.5f, -0.5f,
    0.5f, 0.5f, -0.5f,
    0.5f, 0.5f, -0.5f,
    0.5f, -0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,

    -0.5f, 0.5f, 0.5f, // left face
    -0.5f, 0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,
    -0.5f, -0.5f, 0.5f,
    -0.5f, 0.5f, 0.5f,

    0.5f, 0.5f, 0.5f, // right face
    0.5f, -0.5f, 0.5f,
    0.5f, -0.5f, -0.5f,
    0.5f, -0.5f, -0.5f,
    0.5f, 0.5f, -0.5f,
    0.5f, 0.5f, 0.5f,

    -0.5f, 0.5f, -0.5f, // top face
    -0.5f, 0.5f, 0.5f,
    0.5f, 0.5f, 0.5f,
    0.5f, 0.5f, 0.5f,
    0.5f, 0.5f, -0.5f,
    -0.5f, 0.5f, -0.5f,

    -0.5f, -0.5f, -0.5f, // bottom face
    0.5f, -0.5f, -0.5f,
    0.5f, -0.5f, 0.5f,
    0.5f, -0.5f, 0.5f,
    -0.5f, -0.5f, 0.5f,
    -0.5f, -0.5f, -0.5f};

PlayerRenderer::PlayerRenderer(Shader &shader) : shader(shader)
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    // shader.use();

    modelLoc = glGetUniformLocation(shader.getProgram(), "model");
    viewLoc = glGetUniformLocation(shader.getProgram(), "view");
    projLoc = glGetUniformLocation(shader.getProgram(), "projection");
    colorLoc = glGetUniformLocation(shader.getProgram(), "color");
}

PlayerRenderer::~PlayerRenderer()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    // shader.use();

    modelLoc = glGetUniformLocation(shader.getProgram(), "model");
    viewLoc = glGetUniformLocation(shader.getProgram(), "view");
    projLoc = glGetUniformLocation(shader.getProgram(), "projection");
    colorLoc = glGetUniformLocation(shader.getProgram(), "color");
}

void PlayerRenderer::draw(const glm::vec3 &position, const glm::mat4 &view, const glm::mat4 &projection)
{
    shader.use();

    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR)
    {
        std::cerr << "GL ERROR: " << err << std::endl;
    }

    glm::mat4 model = glm::translate(glm::mat4(1.0f), position);
    model = glm::scale(model, glm::vec3(0.5f));

    if (colorLoc == -1)
        std::cerr << "color uniform not found!" << std::endl;
    glUniform3f(colorLoc, 1.0f, 0.0f, 0.0f); // red for player

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
}