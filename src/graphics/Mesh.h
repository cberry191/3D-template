// Mesh.h
#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <GL/glew.h>
#include "./Vertex.h"

class Mesh
{
public:
    Mesh(const std::vector<Vertex> &vertices, const std::vector<unsigned int> &indices = {});
    ~Mesh();

    void bind() const;
    void draw() const;
    size_t vertexCount;

private:
    GLuint VAO, VBO, EBO;
    size_t indexCount;
    bool useIndices;
};
