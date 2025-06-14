#include <glm/glm.hpp>

class Player
{
public:
    glm::vec3 position = glm::vec3(10.0f, 2.0f, 10.0f); // Initial position
    Player() = default;                                 // Default constructor
    bool selected = false;

    glm::vec3 getPosition() const { return position; }
    void setPosition(const glm::vec3 &pos) { position = pos; }
    void setSelected(bool s) { selected = s; }
    bool isSelected() const { return selected; }

    void walkInASquare()
    {
        static int step = 0;           // Which side of the square
        static int stepsTaken = 0;     // How far along that side we are
        static int stepsPerSide = 100; // How many steps per side
        static float speed = 0.05f;    // Speed per frame

        switch (step)
        {
        case 0: // Move right
            position.x += speed;
            break;
        case 1: // Move forward (increase z)
            position.z += speed;
            break;
        case 2: // Move left
            position.x -= speed;
            break;
        case 3: // Move backward (decrease z)
            position.z -= speed;
            break;
        }

        stepsTaken++;

        if (stepsTaken >= stepsPerSide)
        {
            stepsTaken = 0;
            step = (step + 1) % 4; // Loop through 0–3
        }
    }
};
