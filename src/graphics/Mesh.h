// Mesh.h
#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <GL/glew.h>

class Mesh
{
public:
    Mesh(const float *vertices, size_t size);
    void bind() const;
    void draw() const;
    ~Mesh();
    size_t vertexCount;

private:
    GLuint VAO, VBO;
};
