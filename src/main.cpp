#define GL_SILENCE_DEPRECATION
#define GLM_ENABLE_EXPERIMENTAL
#include "./core/Window.h"
#include "./graphics/Shader.h"
#include "./renderables/GroundRenderer.h"
#include "./renderables/PlayerRenderer.h"
#include "./camera/StrategyCamera.h"
#include "./game/Player.h"
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include <SDL2/SDL.h>
#include <iostream>

bool rayIntersectsAABB(glm::vec3 rayOrigin, glm::vec3 rayDir, glm::vec3 boxMin, glm::vec3 boxMax)
{
    float tmin = (boxMin.x - rayOrigin.x) / rayDir.x;
    float tmax = (boxMax.x - rayOrigin.x) / rayDir.x;
    if (tmin > tmax)
        std::swap(tmin, tmax);

    float tymin = (boxMin.y - rayOrigin.y) / rayDir.y;
    float tymax = (boxMax.y - rayOrigin.y) / rayDir.y;
    if (tymin > tymax)
        std::swap(tymin, tymax);

    if ((tmin > tymax) || (tymin > tmax))
        return false;
    tmin = std::max(tmin, tymin);
    tmax = std::min(tmax, tymax);

    float tzmin = (boxMin.z - rayOrigin.z) / rayDir.z;
    float tzmax = (boxMax.z - rayOrigin.z) / rayDir.z;
    if (tzmin > tzmax)
        std::swap(tzmin, tzmax);

    if ((tmin > tzmax) || (tzmin > tmax))
        return false;

    return true;
};

void handleMouseClick(SDL_Event &event, const glm::mat4 &view, const glm::mat4 &projection, Player &player, int windowX, int windowY)
{
    int mx = event.button.x;
    int my = event.button.y;

    glm::vec4 viewport(0, 0, windowX, windowY);

    // std::cout << "Mouse click at: (" << mx << ", " << my << ")\n";
    // std::cout << "Viewport: " << viewport.x << ", " << viewport.y << ", " << viewport.z << ", " << viewport.w << "\n";

    glm::vec3 rayOrigin = glm::unProject(glm::vec3(mx, viewport.w - my, 0.0f), view, projection, viewport);
    glm::vec3 rayEnd = glm::unProject(glm::vec3(mx, viewport.w - my, 1.0f), view, projection, viewport);
    glm::vec3 rayDir = glm::normalize(rayEnd - rayOrigin);

    // std::cout << "Ray origin: " << glm::to_string(rayOrigin) << "\n";
    // std::cout << "Ray dir:    " << glm::to_string(rayDir) << "\n";

    glm::vec3 boxMin = player.getPosition() - glm::vec3(0.5f);
    glm::vec3 boxMax = player.getPosition() + glm::vec3(0.5f);

    // std::cout << "AABB min: " << glm::to_string(boxMin) << ", max: " << glm::to_string(boxMax) << "\n";

    if (rayIntersectsAABB(rayOrigin, rayDir, boxMin, boxMax))
    {
        std::cout << "Player selected!\n";
        player.setSelected(true);
    }
    else
    {
        std::cout << "Player NOT selected.\n";
        player.setSelected(false);
    }
}

int main()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cerr << "Failed to initialize SDL: " << SDL_GetError() << std::endl;
        return -1;
    };

    Window window("SteppeLord");

    glewExperimental = GL_TRUE;
    glewInit();
    glEnable(GL_CULL_FACE); // Enable back-face culling
    glCullFace(GL_BACK);    // Cull back-facing triangles
    glFrontFace(GL_CCW);    // Counter-clockwise winding is considered front-facing
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // wireframs

    Shader shader("assets/shaders/basic.vert", "assets/shaders/basic.frag");
    GroundRenderer groundRenderer(shader);
    PlayerRenderer playerRenderer(shader);

    StrategyCamera camera;
    Player player;

    camera.setTarget(player.getPosition()); // Set the camera target to the player's position
    camera.zoom(10.0f);                     // pull back
    camera.setPitch(glm::radians(10.0f));   // tilt downward
    camera.rotate(glm::radians(180.0f));

    SDL_Event event;

    Uint32 previousTime = SDL_GetTicks();
    Uint32 currentTime;
    float deltaTime;

    while (window.isRunning())
    {
        currentTime = SDL_GetTicks();
        const float minDelta = 1.0f / 240.0f; // Clamp to 240 FPS max
        const float maxDelta = 1.0f / 15.0f;  // Clamp to 15 FPS min
        deltaTime = glm::clamp((currentTime - previousTime) / 1000.0f, minDelta, maxDelta);
        previousTime = currentTime;

        float cameraSpeed = 250.0f * deltaTime;
        float rotationSpeed = 250.0f * deltaTime;

        glm::mat4 view;

        while (SDL_PollEvent(&event))
        {
            int winW = window.getWidth();
            int winH = window.getHeight();
            float aspect = (float)winW / (float)winH;

            glViewport(0, 0, winW, winH); // keeps GL happy if using immediate rendering
            glm::mat4 projection = glm::perspective(
                glm::radians(45.0f),
                aspect,
                0.1f,
                500.0f);

            if (event.type == SDL_WINDOWEVENT)
            {
                if (event.window.event == SDL_WINDOWEVENT_FOCUS_LOST)
                    std::cout << "Window focus lost\n";
                if (event.window.event == SDL_WINDOWEVENT_FOCUS_GAINED)
                    std::cout << "Window focus gained\n";
                if (event.window.event == SDL_WINDOWEVENT_RESIZED)
                {
                    // TODO : make it so the perspective change is... correct
                    window.resize(event.window.data1, event.window.data2);
                }
            }
            if (event.type == SDL_QUIT)
            {
                window.stop();
                std::cout << "Quit event received. Exiting main loop." << std::endl;
                break;
            }
            if (event.type == SDL_KEYDOWN)
            {
                std::cout << "Key Pressed: " << SDL_GetKeyName(event.key.keysym.sym) << std::endl;

                switch (event.key.keysym.sym)
                {
                case SDLK_w:
                    camera.pan(0.0f, cameraSpeed); // Move forward
                    break;
                case SDLK_s:
                    camera.pan(0.0f, -cameraSpeed); // Move backward
                    break;
                case SDLK_a:
                    camera.pan(cameraSpeed, 0.0f); // Move left
                    break;
                case SDLK_d:
                    camera.pan(-cameraSpeed, 0.0f); // Move right
                    break;
                case SDLK_q:
                {
                    camera.rotate(glm::radians(-rotationSpeed)); // Rotate left
                    break;
                }
                case SDLK_e:
                {
                    camera.rotate(glm::radians(rotationSpeed)); // Rotate right
                    break;
                }
                case SDLK_r:
                    camera.tilt(glm::radians(rotationSpeed)); // Tilt up
                    break;
                case SDLK_f:
                    camera.tilt(glm::radians(-rotationSpeed)); // Tilt down
                    break;
                case SDLK_PLUS:
                case SDLK_KP_PLUS:
                case SDLK_EQUALS:
                    camera.zoom(-cameraSpeed); // Zoom in
                    break;
                case SDLK_MINUS:
                case SDLK_KP_MINUS:
                    camera.zoom(cameraSpeed); // Zoom out
                    break;
                default:
                    std::cout << "Unhandled Key Pressed: " << SDL_GetKeyName(event.key.keysym.sym) << std::endl; // Debug print
                    break;
                }
            }
            if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT)
            {
                glm::mat4 currentView = camera.getViewMatrix();
                handleMouseClick(event, currentView, projection, player, window.getWidth(), window.getHeight()); // Handle mouse click
            }
            if (event.type == SDL_MOUSEWHEEL)
            {
                float zoomAmount = event.wheel.y * cameraSpeed; // Adjust zoom speed as needed
                camera.zoom(zoomAmount);                        // Zoom in or out based on scroll direction
                // std::cout << "Mouse Wheel Scrolled: " << zoomAmount << std::endl; // Debug print
            }
        }
        if (player.isSelected())
        {
            camera.setTarget(player.getPosition());
        }
        view = camera.getViewMatrix(); // Update the view matrix after handling events
        int winW = window.getWidth();
        int winH = window.getHeight();
        glm::mat4 projection = glm::perspective(
            glm::radians(45.0f),
            (float)winW / (float)winH,
            0.1f,
            500.0f);

        glClearColor(0.2f, 0.3f, 0.4f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        groundRenderer.draw(view, projection);
        player.walkInASquare();                                      // Simple renderer call
        playerRenderer.draw(player.getPosition(), view, projection); // Player renderer call

        window.swapBuffers();
    }
    return 0;
}
