#include "CameraController.h"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream> // For debug prints

void CameraController::processInput(SDL_Event &event, glm::vec3 &cameraPos, glm::vec3 &cameraFront, glm::vec3 &cameraUp, float deltaTime)
{
    float cameraSpeed = 2.5f * deltaTime;
    float rotationSpeed = 50.0f * deltaTime;

    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_KEYDOWN)
        {
            std::cout << "Key Pressed: " << SDL_GetKeyName(event.key.keysym.sym) << std::endl; // Debug print

            switch (event.key.keysym.sym)
            {
            case SDLK_w:
                cameraPos += cameraSpeed * cameraFront;
                std::cout << "Moving Forward" << std::endl; // Debug print
                break;
            case SDLK_s:
                cameraPos -= cameraSpeed * cameraFront;
                std::cout << "Moving Backward" << std::endl; // Debug print
                break;
            case SDLK_a:
                cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
                std::cout << "Moving Left" << std::endl; // Debug print
                break;
            case SDLK_d:
                cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
                std::cout << "Moving Right" << std::endl; // Debug print
                break;
            case SDLK_q:
                cameraFront = glm::mat3(glm::rotate(glm::mat4(1.0f), glm::radians(rotationSpeed), cameraUp)) * cameraFront;
                std::cout << "Rotating Left" << std::endl; // Debug print
                break;
            case SDLK_e:
                cameraFront = glm::mat3(glm::rotate(glm::mat4(1.0f), glm::radians(-rotationSpeed), cameraUp)) * cameraFront;
                std::cout << "Rotating Right" << std::endl; // Debug print
                break;
            }
        }
    }
}