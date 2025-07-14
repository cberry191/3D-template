#include <iostream>
#include "Game.h"
#include <glad/glad.h>

std::vector<Vertex> cubeVertices = {
    // Position               Normal              UV
    {{-0.5f, -0.5f, -0.5f}, {}, {}}, // 0
    {{0.5f, -0.5f, -0.5f}, {}, {}},  // 1
    {{0.5f, 0.5f, -0.5f}, {}, {}},   // 2
    {{-0.5f, 0.5f, -0.5f}, {}, {}},  // 3
    {{-0.5f, -0.5f, 0.5f}, {}, {}},  // 4
    {{0.5f, -0.5f, 0.5f}, {}, {}},   // 5
    {{0.5f, 0.5f, 0.5f}, {}, {}},    // 6
    {{-0.5f, 0.5f, 0.5f}, {}, {}},   // 7
};

std::vector<unsigned int> cubeIndices = {
    // Front face
    4, 5, 6,
    6, 7, 4,
    // Back face
    1, 0, 3,
    3, 2, 1,
    // Left face
    0, 4, 7,
    7, 3, 0,
    // Right face
    5, 1, 2,
    2, 6, 5,
    // Top face
    3, 7, 6,
    6, 2, 3,
    // Bottom face
    0, 1, 5,
    5, 4, 0};

std::vector<Vertex> groundVertices = {
    {{-0.5f, 0.0f, -0.5f}, {0, 1, 0}, {0.0f, 0.0f}}, // 0
    {{0.5f, 0.0f, -0.5f}, {0, 1, 0}, {1.0f, 0.0f}},  // 1
    {{0.5f, 0.0f, 0.5f}, {0, 1, 0}, {1.0f, 1.0f}},   // 2
    {{-0.5f, 0.0f, 0.5f}, {0, 1, 0}, {0.0f, 1.0f}},  // 3
};

std::vector<unsigned int> groundIndices = {
    0, 1, 2, // First triangle
    0, 2, 3  // Second triangle
};

Game::Game(Window &window)
    : window(window)
{
}

void Game::testScene()
{
    Mesh *groundMesh = new Mesh(groundVertices, groundIndices);
    Mesh *cubeMesh = new Mesh(cubeVertices, cubeIndices);

    Entity ground("Ground");
    ground.setPosition({0, 0, 0});
    ground.renderable = new RenderableComponent(groundMesh, shader);
    ground.colour = glm::vec3(0.5f, 1.0f, 0.0f);
    ground.scale = glm::vec3(100.0f, 1.0f, 100.0f);
    entities.push_back(ground);

    Entity cube("Cube");
    cube.setPosition({10, 5.0f, 0});
    cube.renderable = new RenderableComponent(cubeMesh, shader);
    cube.colour = glm::vec3(1.0f, 0.0f, 1.0f);
    entities.push_back(cube);

    Entity player("Player");
    player.setPosition({-5.0f, 1.01f, -5.0f});
    player.model = new Model(shader);
    player.model->loadFromFile("assets/models/CharacterBase.glb");
    player.colour = glm::vec3(0.0f, 0.0f, 1.0f);
    entities.push_back(player);
}

void Game::setup()
{
    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
    {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        exit(EXIT_FAILURE);
    }

    framebuffer.init(window.getWidth(), window.getHeight());

    float quadVertices[] = {
        // positions   // texCoords
        -1.0f, 1.0f, 0.0f, 1.0f,
        -1.0f, -1.0f, 0.0f, 0.0f,
        1.0f, -1.0f, 1.0f, 0.0f,

        -1.0f, 1.0f, 0.0f, 1.0f,
        1.0f, -1.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 1.0f, 1.0f};

    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &quadVBO);
    glBindVertexArray(quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)(2 * sizeof(float)));

    // glEnable(GL_CULL_FACE); // Enable back-face culling
    glCullFace(GL_BACK); // Cull back-facing triangles
    glFrontFace(GL_CCW); // Counter-clockwise winding is considered front-facing
    // glEnable(GL_DEPTH_TEST);
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // wireframs

    shader = new Shader("assets/shaders/basic.vert", "assets/shaders/basic.frag");
    screenShader = new Shader("assets/shaders/screen.vert", "assets/shaders/screen.frag");
    screenShader->use();
    screenShader->setInt("screenTexture", 0); // Texture unit 0

    testScene();

    camera.setTarget(glm::vec3(0.0f, 0.0f, 0.0f));
    camera.zoom(5.0f);
    camera.setPitch(glm::radians(10.0f));
    camera.rotate(glm::radians(180.0f));

    Uint32 previousTime = SDL_GetTicks();
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
        std::cout << entity.name << ": selected!\n";
        entity.setSelected(true);
    }
    else
    {
        entity.setSelected(false);
    }
}

void Game::run()
{
    while (window.isRunning())
    {
        Uint32 currentTime = SDL_GetTicks();
        float deltaTime = glm::clamp((currentTime - previousTime) / 1000.0f, 1.0f / 240.0f, 1.0f / 15.0f);
        previousTime = currentTime;

        cameraSpeed = 200.0f * deltaTime;
        rotationSpeed = 300.0f * deltaTime;

        handleEvents();
        update(deltaTime);
        view = camera.getViewMatrix();

        int winW = window.getWidth();
        int winH = window.getHeight();
        float aspect = (float)winW / (float)winH;
        glViewport(0, 0, winW, winH);

        projection = glm::perspective(
            glm::radians(45.0f),
            aspect,
            0.1f,
            500.0f);

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
                // flag max camera distance from origin
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
                camera.zoom(cameraSpeed);
                break;
            default:
                std::cout << "Unhandled Key Pressed: " << SDL_GetKeyName(event.key.keysym.sym) << std::endl;
                break;
            }
        }
        if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT)
        {
            glm::mat4 currentView = camera.getViewMatrix();
            // TODO limit no of entities to check for selection
            for (auto &entity : entities)
            {
                handleMouseClick(event, currentView, projection, entity, window.getWidth(), window.getHeight());
                if (entity.isSelected())
                {
                    camera.setTarget(entity.getPosition());
                    selectedEntity = &entity;
                }
            }
        }
        if (event.type == SDL_MOUSEWHEEL)
        {
            float zoomAmount = event.wheel.y * cameraSpeed;
            camera.zoom(zoomAmount);
        }
    }
}

void Game::render()
{
    // --- Step 1: Render the scene into the framebuffer ---
    framebuffer.bind();
    glEnable(GL_DEPTH_TEST); // Needed for proper 3D rendering

    glClearColor(0.2f, 0.3f, 0.4f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for (auto &entity : entities)
    {
        entity.draw(projection, view);
    }

    framebuffer.unbind();

    // --- Step 2: Render the framebuffer texture to a fullscreen quad ---
    glDisable(GL_DEPTH_TEST); // Disable depth for screen-space quad

    screenShader->use();        // Shader that draws a texture to screen
    glBindVertexArray(quadVAO); // Fullscreen quad VAO
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, framebuffer.getTextureId());

    glDrawArrays(GL_TRIANGLES, 0, 6); // 6 verts = 2 triangles
}

void Game::update(float deltaTime)
{
    // Move AI, physics, animation, or scripting here

    // For now, maybe update player/cube logic here if needed
}