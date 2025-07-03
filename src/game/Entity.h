#include "../graphics/RenderableComponent.h"
#include <iostream>
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <functional>

class Entity
{
public:
    // virtual ~Entity() = default;
    glm::vec3 position = glm::vec3(0.0f, .0f, 0.0f);
    bool selected = false;

    glm::vec3 getPosition() const { return position; }
    void setPosition(const glm::vec3 &pos) { position = pos; }
    void setSelected(bool s) { selected = s; }
    bool isSelected() const { return selected; }

    virtual int getId() const { return 0; }
    virtual std::string getName() const { return "Entity"; }
    virtual std::string getType() const { return "Generic"; }

    RenderableComponent *renderable = nullptr;

    glm::vec3 colour = glm::vec3(1.0f);
    glm::vec3 scale = glm::vec3(1.0f);

    void draw(glm::mat4 projection, glm::mat4 view)
    {
        // if (!isVisible(view, projection))
        //     return;

        if (renderable)
        {
            glm::mat4 model = glm::translate(glm::mat4(1.0f), position);
            model = glm::scale(model, scale);
            renderable->draw(model, view, projection, colour);
        }
    }

    std::function<void(Entity &)> updateFn;

    void update()
    {
        if (updateFn)
            updateFn(*this);
    }
};