#include "Model.h"

#include <glm/gtc/type_ptr.hpp>

Model::Model(Shader *shader) : shader(shader)
{
    if (!shader)
    {
        std::cerr << "Shader is null in Model constructor.\n";
    }
}

void Model::draw(glm::mat4 modelMatrix, glm::mat4 view, glm::mat4 projection, glm::vec3 color)
{
    if (!shader)
        return;

    shader->use();
    shader->setVec3("color", color); // you may have this method or set manually
    shader->setMat4("model", modelMatrix);
    shader->setMat4("view", view);
    shader->setMat4("projection", projection);

    for (auto &mesh : meshes)
    {
        mesh->bind();
        mesh->draw();
    }
}

bool Model::loadFromFile(const std::string &filename)
{
    if (!loader.LoadBinaryFromFile(&model, &err, &warn, filename))
    {
        std::cerr << "Failed to load model: " << err << "\n";
        return false;
    }
    if (!warn.empty())
    {
        std::cout << "Warning: " << warn << "\n";
    }
    std::cout << "Model loaded successfully from " << filename << "\n";

    for (const auto &mesh : model.meshes)
    {
        for (const auto &primitive : mesh.primitives)
        {
            std::vector<Vertex> vertices;
            std::vector<unsigned int> indices;

            // Extract attributes
            const auto &positionAccessor = model.accessors[primitive.attributes.at("POSITION")];
            const auto &positionBufferView = model.bufferViews[positionAccessor.bufferView];
            const auto &positionBuffer = model.buffers[positionBufferView.buffer];

            const float *positions = reinterpret_cast<const float *>(
                &positionBuffer.data[positionBufferView.byteOffset + positionAccessor.byteOffset]);

            for (size_t i = 0; i < positionAccessor.count; ++i)
            {
                Vertex v;
                v.position = glm::vec3(
                    positions[i * 3 + 0],
                    positions[i * 3 + 1],
                    positions[i * 3 + 2]);
                v.normal = glm::vec3(0.0f);    // stub
                v.texCoords = glm::vec2(0.0f); // stub
                vertices.push_back(v);
            }

            // Extract indices
            if (primitive.indices >= 0)
            {
                const auto &indexAccessor = model.accessors[primitive.indices];
                const auto &indexBufferView = model.bufferViews[indexAccessor.bufferView];
                const auto &indexBuffer = model.buffers[indexBufferView.buffer];

                const void *dataPtr = &indexBuffer.data[indexBufferView.byteOffset + indexAccessor.byteOffset];
                switch (indexAccessor.componentType)
                {
                case TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT:
                {
                    const uint16_t *buf = reinterpret_cast<const uint16_t *>(dataPtr);
                    for (size_t i = 0; i < indexAccessor.count; ++i)
                        indices.push_back(static_cast<unsigned int>(buf[i]));
                    break;
                }
                case TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT:
                {
                    const uint32_t *buf = reinterpret_cast<const uint32_t *>(dataPtr);
                    for (size_t i = 0; i < indexAccessor.count; ++i)
                        indices.push_back(buf[i]);
                    break;
                }
                case TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE:
                {
                    const uint8_t *buf = reinterpret_cast<const uint8_t *>(dataPtr);
                    for (size_t i = 0; i < indexAccessor.count; ++i)
                        indices.push_back(buf[i]);
                    break;
                }
                default:
                    std::cerr << "Unsupported index component type.\n";
                }
            }

            Mesh *newMesh = new Mesh(vertices, indices);
            meshes.push_back(newMesh);
        }
    }

    return true;
}