#include "Window.h"
#include <iostream>

Window::Window(const char *title, int width, int height, bool fullscreen)
    : width(width), height(height)
{
    Uint32 windowFlags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE;

    SDL_Init(SDL_INIT_VIDEO);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    if (fullscreen)
    {
        windowFlags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
        SDL_DisplayMode dm;
        if (SDL_GetDesktopDisplayMode(0, &dm) == 0)
        {
            this->width = dm.w;
            this->height = dm.h;
        }
    }

    window = SDL_CreateWindow(title,
                              SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                              this->width, this->height,
                              windowFlags);

    if (!window)
    {
        std::cerr << "Failed to create window: " << SDL_GetError() << std::endl;
        exit(1);
    }

    glContext = SDL_GL_CreateContext(window);
    SDL_GL_SetSwapInterval(1); // vsync
    running = true;
}

void Window::stop()
{
    running = false;
}

Window::~Window()
{
    SDL_DestroyWindow(window);
    SDL_Quit();
}

bool Window::isRunning()
{
    return running;
}

void Window::swapBuffers()
{
    SDL_GL_SwapWindow(window);
}

bool Window::isKeyPressed(SDL_Keycode key)
{
    const Uint8 *state = SDL_GetKeyboardState(nullptr);
    return state[SDL_GetScancodeFromKey(key)];
}
