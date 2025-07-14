#include "../core/Window.h"
#include "../camera/StrategyCamera.h"
#include "../graphics/Shader.h"
#include "../graphics/Framebuffer.h"
#include "../game/Entity.h"
#include <glad/glad.h>
#include <vector>
#include <cstdint>

class Game
{
    std::vector<Entity> entities;

public:
    Game(Window &window);
    void setup(); // Init resources, entities
    void run();   // Game loop

    Entity *selectedEntity = nullptr;

private:
    Window &window;
    Framebuffer framebuffer;

    StrategyCamera camera;
    float cameraSpeed = 0.0f;
    float rotationSpeed = 0.0f;

    Shader *shader = nullptr;
    Shader *screenShader = nullptr;

    glm::mat4 view;
    glm::mat4 projection;

    Uint32 previousTime = 0;

    void handleEvents();
    void update(float deltaTime); // Game logic
    void render();

    void testScene();

    GLuint quadVAO, quadVBO;
};