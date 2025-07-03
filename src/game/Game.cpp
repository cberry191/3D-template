#include <iostream>
#include "Game.h"

float groundVertices[] = {
    // Ground rectangle (two triangles forming a rectangle)
    -0.5f, -0.5f, 0.0f,
    0.5f, -0.5f, 0.0f,
    0.5f, 0.5f, 0.0f,

    -0.5f, -0.5f, 0.0f,
    0.5f, 0.5f, 0.0f,
    -0.5f, 0.5f, 0.0f};

float cubeVertices[] = {
    // Cube vertices (36 total, 12 triangles)
    -0.5f, -0.5f, 0.5f, // front face
    0.5f, -0.5f, 0.5f,
    0.5f, 0.5f, 0.5f,
    0.5f, 0.5f, 0.5f,
    -0.5f, 0.5f, 0.5f,
    -0.5f, -0.5f, 0.5f,

    -0.5f, -0.5f, -0.5f, // back face
    -0.5f, 0.5f, -0.5f,
    0.5f, 0.5f, -0.5f,
    0.5f, 0.5f, -0.5f,
    0.5f, -0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,

    -0.5f, 0.5f, 0.5f, // left face
    -0.5f, 0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,
    -0.5f, -0.5f, 0.5f,
    -0.5f, 0.5f, 0.5f,

    0.5f, 0.5f, 0.5f, // right face
    0.5f, -0.5f, 0.5f,
    0.5f, -0.5f, -0.5f,
    0.5f, -0.5f, -0.5f,
    0.5f, 0.5f, -0.5f,
    0.5f, 0.5f, 0.5f,

    -0.5f, 0.5f, -0.5f, // top face
    -0.5f, 0.5f, 0.5f,
    0.5f, 0.5f, 0.5f,
    0.5f, 0.5f, 0.5f,
    0.5f, 0.5f, -0.5f,
    -0.5f, 0.5f, -0.5f,

    -0.5f, -0.5f, -0.5f, // bottom face
    0.5f, -0.5f, -0.5f,
    0.5f, -0.5f, 0.5f,
    0.5f, -0.5f, 0.5f,
    -0.5f, -0.5f, 0.5f,
    -0.5f, -0.5f, -0.5f};

Game::Game(Window &window) : window(window) {}

void Game::setup()
{
    glewExperimental = GL_TRUE;
    glewInit();
    // glEnable(GL_CULL_FACE); // Enable back-face culling
    glCullFace(GL_BACK); // Cull back-facing triangles
    glFrontFace(GL_CCW); // Counter-clockwise winding is considered front-facing
    glEnable(GL_DEPTH_TEST);
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // wireframs

    shader = new Shader("assets/shaders/basic.vert", "assets/shaders/basic.frag");

    Mesh *groundMesh = new Mesh(groundVertices, sizeof(groundVertices));
    Mesh *cubeMesh = new Mesh(cubeVertices, sizeof(cubeVertices));

    Entity ground;
    ground.setPosition({0, 0, 0});
    ground.renderable = new RenderableComponent(groundMesh, shader);
    ground.colour = glm::vec3(0.0f, 1.0f, 0.0f);
    ground.scale = glm::vec3(20.0f, 1.0f, 20.0f);
    entities.push_back(ground);

    Entity player;
    player.setPosition({0, 0, 0});
    player.renderable = new RenderableComponent(cubeMesh, shader);
    player.colour = glm::vec3(1.0f, 0.0f, 0.0f);
    entities.push_back(player);

    camera.setTarget(glm::vec3(0.0f, .0f, 3.0f)); // Set the camera target to the player's position
    camera.zoom(5.0f);                            // pull back
    camera.setPitch(glm::radians(0.0f));          // tilt downward
    camera.rotate(glm::radians(0.0f));

    Uint32 previousTime = SDL_GetTicks();
}

void Game::run()
{
    while (window.isRunning())
    {
        Uint32 currentTime = SDL_GetTicks();
        float deltaTime = glm::clamp((currentTime - previousTime) / 1000.0f, 1.0f / 240.0f, 1.0f / 15.0f);
        previousTime = currentTime;

        cameraSpeed = 250.0f * deltaTime;
        rotationSpeed = 250.0f * deltaTime;

        view = camera.getViewMatrix();
        // view = glm::lookAt(
        //     glm::vec3(0, 0, 3), // eye
        //     glm::vec3(0, 0, 0), // target
        //     glm::vec3(0, 1, 0)  // up
        // );

        int winW = window.getWidth();
        int winH = window.getHeight();
        float aspect = (float)winW / (float)winH;
        glViewport(0, 0, winW, winH); // keeps GL happy if using immediate rendering

        projection = glm::perspective(
            glm::radians(45.0f),
            aspect,
            0.1f,
            100.0f);

        handleEvents();
        // update(deltaTime);
        render();

        window.swapBuffers();
    }
}

void Game::handleEvents()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
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
            // TODO change the below to take an entity instead of a player

            // handleMouseClick(event, currentView, projection, player, window.getWidth(), window.getHeight());
            // if (player.isSelected())
            // {
            //     camera.setTarget(player.getPosition());
            // }
        }
        if (event.type == SDL_MOUSEWHEEL)
        {
            float zoomAmount = event.wheel.y * cameraSpeed; // Adjust zoom speed as needed
            camera.zoom(zoomAmount);                        // Zoom in or out based on scroll direction
            // std::cout << "Mouse Wheel Scrolled: " << zoomAmount << std::endl; // Debug print
        }
    }
}

void Game::render()
{
    glClearColor(0.2f, 0.3f, 0.4f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for (auto &entity : entities)
    {
        entity.draw(projection, view);
    }
}

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

void handleMouseClick(SDL_Event &event, const glm::mat4 &view, const glm::mat4 &projection, Entity &entity, int windowX, int windowY)
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

    glm::vec3 boxMin = entity.getPosition() - glm::vec3(0.5f);
    glm::vec3 boxMax = entity.getPosition() + glm::vec3(0.5f);

    // std::cout << "AABB min: " << glm::to_string(boxMin) << ", max: " << glm::to_string(boxMax) << "\n";

    if (rayIntersectsAABB(rayOrigin, rayDir, boxMin, boxMax))
    {
        std::cout << "Player selected!\n";
        entity.setSelected(true);
    }
    else
    {
        std::cout << "Player NOT selected.\n";
        entity.setSelected(false);
    }
}