#include <glm/glm.hpp>

class Renderable
{
public:
    virtual void draw(const glm::mat4 &view, const glm::mat4 &proj) = 0;
};
