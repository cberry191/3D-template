#pragma once
#include <glad/glad.h>

class Framebuffer
{
public:
    Framebuffer() = default;
    ~Framebuffer();

    void init(int width, int height);

    void bind();
    void unbind();
    GLuint getTextureId() const;

private:
    GLuint FBO = 0;
    GLuint textureColorBuffer = 0;
    GLuint RBO = 0;
};