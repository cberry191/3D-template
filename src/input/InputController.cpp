#include "InputController.h"
#include <iostream>
#include <SDL2/SDL.h>
#include "../camera/StrategyCamera.h"
#include "../core/Window.h"

void InputController::processInput(SDL_Event &event, StrategyCamera &camera, Window &window)
{
    if (event.type == SDL_QUIT)
    {
        window.stop();                                                       // Stop the window if quit event is received
        std::cout << "Quit event received. Exiting main loop." << std::endl; // Debug print
    }
    else if (event.type == SDL_KEYDOWN)
    {
        std::cout << "Key Pressed: " << SDL_GetKeyName(event.key.keysym.sym) << std::endl; // Debug print

        switch (event.key.keysym.sym)
        {
        case SDLK_w:
            camera.pan(0.0f, cameraSpeed);              // Move forward
            std::cout << "Moving Forward" << std::endl; // Debug print
            break;
        case SDLK_s:
            camera.pan(0.0f, -cameraSpeed);              // Move backward
            std::cout << "Moving Backward" << std::endl; // Debug print
            break;
        case SDLK_a:
            camera.pan(-cameraSpeed, 0.0f);          // Move left
            std::cout << "Moving Left" << std::endl; // Debug print
            break;
        case SDLK_d:
            camera.pan(cameraSpeed, 0.0f);            // Move right
            std::cout << "Moving Right" << std::endl; // Debug print
            break;
        case SDLK_q:
            camera.rotate(glm::radians(-rotationSpeed)); // Rotate left
            std::cout << "Orbiting Left" << std::endl;
            break;
        case SDLK_e:
            camera.rotate(glm::radians(rotationSpeed)); // Rotate right
            std::cout << "Orbiting Right" << std::endl;
            break;
        case SDLK_r:
            camera.tilt(glm::radians(rotationSpeed)); // Tilt up
            std::cout << "Tilting Up" << std::endl;   // Debug print
            break;
        case SDLK_f:
            camera.tilt(glm::radians(-rotationSpeed)); // Tilt down
            std::cout << "Tilting Down" << std::endl;  // Debug print
            break;
        case SDLK_PLUS:
        case SDLK_KP_PLUS:
        case SDLK_EQUALS:
            camera.zoom(-cameraSpeed);              // Zoom in
            std::cout << "Zooming In" << std::endl; // Debug print
            break;
        case SDLK_MINUS:
        case SDLK_KP_MINUS:
            camera.zoom(cameraSpeed);                // Zoom out
            std::cout << "Zooming Out" << std::endl; // Debug print
            break;
        default:
            std::cout << "Unhandled key: " << SDL_GetKeyName(event.key.keysym.sym) << std::endl; // Debug print
            break;
        }
    }
}
