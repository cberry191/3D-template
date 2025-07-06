#include "../graphics/RenderableComponent.h"
#include "../graphics/Model.h"
#include <iostream>
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
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
    Model *model = nullptr;

    glm::vec3 colour = glm::vec3(1.0f);
    glm::vec3 scale = glm::vec3(1.0f);

    void draw(glm::mat4 projection, glm::mat4 view)
    {
        glm::mat4 baseModel = glm::translate(glm::mat4(1.0f), position);
        baseModel = glm::scale(baseModel, scale);

        // if (!isVisible(view, projection))
        //     return;

        if (renderable)
        {
            renderable->draw(baseModel, view, projection, colour);
        }
        else if (model)
        {
            model->draw(baseModel, view, projection, colour);
        }
    }

    // void rotate(float angle, glm::vec3 axis)
    // {
    //     model = glm::rotate(model, angle, axis);
    // }

    std::function<void(Entity &)> updateFn;

    void update()
    {
        if (updateFn)
            updateFn(*this);
    }
};