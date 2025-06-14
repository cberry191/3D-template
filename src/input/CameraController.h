#ifndef CAMERACONTROLLER_H
#define CAMERACONTROLLER_H

#include <glm/glm.hpp>
#include <SDL2/SDL.h>

class CameraController
{
public:
    void processInput(SDL_Event &event, glm::vec3 &cameraPos, glm::vec3 &cameraFront, glm::vec3 &cameraUp, float deltaTime);
};

#endif