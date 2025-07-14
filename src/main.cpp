#define GL_SILENCE_DEPRECATION
#define GLM_ENABLE_EXPERIMENTAL
#include "./core/Window.h"
#include "./game/Game.h"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include <SDL2/SDL.h>
#include <iostream>

#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define TINYGLTF_NO_INCLUDE_JSON

#include "json.hpp"
#include "tiny_gltf.h"

int main()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cerr << "Failed to initialize SDL: " << SDL_GetError() << std::endl;
        return -1;
    };

    Window window("SteppeLord");

    Game game(window);
    game.setup();
    game.run();

    return 0;
}
