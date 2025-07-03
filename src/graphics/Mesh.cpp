// Mesh.cpp
#include <iostream>
#include "Mesh.h"

Mesh::Mesh(const float *vertices, size_t size)
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    vertexCount = size / (3 * sizeof(float));
    // std::cout << "vertexCount: " << vertexCount << "\n";
}

void Mesh::bind() const
{
    glBindVertexArray(VAO);
}

void Mesh::draw() const
{
    glDrawArrays(GL_TRIANGLES, 0, vertexCount);

    GLenum err = glGetError();
    if (err != GL_NO_ERROR)
        std::cerr << "GL ERROR: " << err << "\n";
}

Mesh::~Mesh()
{
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);
}
