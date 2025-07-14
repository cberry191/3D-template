#include <vector>
#include <tiny_gltf.h>
#include <iostream>
#include "Mesh.h"
#include "Shader.h"

class Model
{
public:
    Model(Shader *shader);

    bool loadFromFile(const std::string &filename);

    void draw(glm::mat4 modelMatrix, glm::mat4 view, glm::mat4 projection, glm::vec3 color, bool isSelected = false);

    void setShader(Shader *s) { shader = s; }

    int getMeshCount() const { return static_cast<int>(meshes.size()); }

private:
    std::vector<Mesh *> meshes;
    Shader *shader = nullptr;

    tinygltf::Model model;
    tinygltf::TinyGLTF loader;
    std::string err, warn;
};