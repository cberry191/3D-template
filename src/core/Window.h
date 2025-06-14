#pragma once
#include <SDL2/SDL.h>

class Window
{
public:
    Window(const char *title, int width = 1280, int height = 720, bool fullscreen = false);
    ~Window();

    void stop();
    bool isRunning();
    void processEvents();
    void swapBuffers();
    bool isKeyPressed(SDL_Keycode key);

    int width, height;
    int getWidth() const { return width; }
    int getHeight() const { return height; }
    void resize(int newWidth, int newHeight)
    {
        width = newWidth;
        height = newHeight;
    }

private:
    SDL_Window *window;
    SDL_GLContext glContext;
    bool running;
};
