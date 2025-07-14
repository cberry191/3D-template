#include "./Framebuffer.h"
#include <iostream>

void Framebuffer::init(int width, int height)
{
    std::cout << "Generating framebuffer..." << std::endl;

    glGenFramebuffers(1, &FBO);
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);

    // Create texture to render into
    glGenTextures(1, &textureColorBuffer);
    glBindTexture(GL_TEXTURE_2D, textureColorBuffer);

    if (width <= 0 || height <= 0)
    {
        std::cerr << "Invalid framebuffer size: " << width << "x" << height << std::endl;
        return;
    }
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorBuffer, 0);

    // Create renderbuffer for depth + stencil
    glGenRenderbuffers(1, &RBO);
    glBindRenderbuffer(GL_RENDERBUFFER, RBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cerr << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    std::cout << "Framebuffer init: " << width << "x" << height << std::endl;
    std::cout << "Framebuffer initialized." << std::endl;
}

Framebuffer::~Framebuffer()
{
    if (FBO)
        glDeleteFramebuffers(1, &FBO);
    if (textureColorBuffer)
        glDeleteTextures(1, &textureColorBuffer);
    if (RBO)
        glDeleteRenderbuffers(1, &RBO);
}

void Framebuffer::bind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);
}

void Framebuffer::unbind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

GLuint Framebuffer::getTextureId() const
{
    return textureColorBuffer;
}
