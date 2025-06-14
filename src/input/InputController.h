#include <iostream>
#include <SDL2/SDL.h>
#include "../camera/StrategyCamera.h"
#include "../game/Player.h"
#include "../core/Window.h"

class InputController
{
private:
    SDL_Event event;     // SDL event structure to handle input events
    float cameraSpeed;   // Speed of camera movement
    float rotationSpeed; // Speed of camera rotation
public:
    InputController(float camSpeed = 250.0f, float rotSpeed = 250.0f)
        : cameraSpeed(camSpeed), rotationSpeed(rotSpeed) {}

    void processInput(SDL_Event &event, StrategyCamera &camera, Window &window) {}
    bool rayIntersectsAABB(glm::vec3 rayOrigin, glm::vec3 rayDir, glm::vec3 boxMin, glm::vec3 boxMax) {}
    void handleMouseClick(SDL_Event &event, const glm::mat4 &view, const glm::mat4 &projection, Player &player, int windowX, int windowY) {}
};