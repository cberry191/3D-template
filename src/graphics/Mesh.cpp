// Mesh.cpp
#include <iostream>
#include "Mesh.h"

// Mesh::Mesh(const float *vertices, size_t size)
// {
//     glGenVertexArrays(1, &VAO);
//     glGenBuffers(1, &VBO);

//     glBindVertexArray(VAO);
//     glBindBuffer(GL_ARRAY_BUFFER, VBO);
//     glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);

//     glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
//     glEnableVertexAttribArray(0);

//     glBindBuffer(GL_ARRAY_BUFFER, 0);
//     glBindVertexArray(0);

//     vertexCount = size / (3 * sizeof(float));
//     std::cout << "vertexCount: " << vertexCount << "\n";
// }

Mesh::Mesh(const std::vector<Vertex> &vertices, const std::vector<unsigned int> &indices)
{
    useIndices = !indices.empty();
    indexCount = useIndices ? indices.size() : vertices.size();

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    if (useIndices)
        glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    // Vertex buffer
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

    // Index buffer
    if (useIndices)
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
    }

    // Vertex attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)0); // position
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)(offsetof(Vertex, normal))); // normal
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)(offsetof(Vertex, texCoords))); // texCoord
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
}

void Mesh::bind() const
{
    glBindVertexArray(VAO);
}

// void Mesh::draw() const
// {
//     glDrawArrays(GL_TRIANGLES, 0, vertexCount);

//     GLenum err = glGetError();
//     if (err != GL_NO_ERROR)
//         std::cerr << "GL ERROR: " << err << "\n";
// }

// Mesh::~Mesh()
// {
//     glDeleteBuffers(1, &VBO);
//     glDeleteVertexArrays(1, &VAO);
// }

void Mesh::draw() const
{
    glBindVertexArray(VAO);
    if (useIndices)
    {
        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indexCount), GL_UNSIGNED_INT, 0);
    }
    else
    {
        glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(indexCount));
    }
    glBindVertexArray(0);
}

Mesh::~Mesh()
{
    glDeleteBuffers(1, &VBO);
    if (useIndices)
        glDeleteBuffers(1, &EBO);
    glDeleteVertexArrays(1, &VAO);
}