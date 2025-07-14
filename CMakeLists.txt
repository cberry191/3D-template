cmake_minimum_required(VERSION 3.16)
project(SteppeLord)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_PREFIX_PATH "/opt/homebrew")

# copy assets folder automatically on build
file(COPY ${CMAKE_SOURCE_DIR}/assets DESTINATION ${CMAKE_BINARY_DIR})

find_package(SDL2 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(glm REQUIRED)

link_directories(/opt/homebrew/lib)

add_executable(SteppeLord
src/main.cpp
src/core/Window.cpp
src/graphics/Shader.cpp
src/graphics/RenderableComponent.cpp
src/graphics/Mesh.cpp
src/graphics/Model.cpp
src/graphics/Framebuffer.cpp
src/camera/StrategyCamera.cpp
src/game/Game.cpp
external/glad/src/glad.c
)

target_include_directories(SteppeLord PRIVATE
    ${CMAKE_SOURCE_DIR}/include    
    /opt/homebrew/include/SDL2
    /opt/homebrew/include
    ${GLM_INCLUDE_DIRS}
    ${CMAKE_SOURCE_DIR}/external
    ${CMAKE_SOURCE_DIR}/external/glad/include
)

target_link_libraries(SteppeLord
    SDL2
    OpenGL::GL
)

set_target_properties(SteppeLord PROPERTIES
    BUILD_RPATH "/opt/homebrew/lib"
)