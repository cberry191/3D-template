#include "../graphics/Mesh.h"
#include "../graphics/Shader.h"
#include <glm/glm.hpp>
#include <vector>

class RenderableComponent
{
public:
    RenderableComponent(Mesh *mesh, Shader *shader);
    void draw(glm::mat4 model, glm::mat4 view, glm::mat4 projection, glm::vec3 color, bool isSelected = false);
    std::vector<float> vertices;
    Mesh *mesh;
    Shader *shader;

private:
    GLuint VAO, VBO;
    GLint modelLoc, viewLoc, projLoc, colorLoc;
};