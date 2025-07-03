// #include <glm/glm.hpp>

// class Player : public Entity
// {
// public:
//     Player()
//     {
//         position = glm::vec3(10.0f, 2.0f, 10.0f); // Initial position
//     }
//     float cubeVertices[] = {
//         // Cube vertices (36 total, 12 triangles)
//         -0.5f, -0.5f, 0.5f, // front face
//         0.5f, -0.5f, 0.5f,
//         0.5f, 0.5f, 0.5f,
//         0.5f, 0.5f, 0.5f,
//         -0.5f, 0.5f, 0.5f,
//         -0.5f, -0.5f, 0.5f,

//         -0.5f, -0.5f, -0.5f, // back face
//         -0.5f, 0.5f, -0.5f,
//         0.5f, 0.5f, -0.5f,
//         0.5f, 0.5f, -0.5f,
//         0.5f, -0.5f, -0.5f,
//         -0.5f, -0.5f, -0.5f,

//         -0.5f, 0.5f, 0.5f, // left face
//         -0.5f, 0.5f, -0.5f,
//         -0.5f, -0.5f, -0.5f,
//         -0.5f, -0.5f, -0.5f,
//         -0.5f, -0.5f, 0.5f,
//         -0.5f, 0.5f, 0.5f,

//         0.5f, 0.5f, 0.5f, // right face
//         0.5f, -0.5f, 0.5f,
//         0.5f, -0.5f, -0.5f,
//         0.5f, -0.5f, -0.5f,
//         0.5f, 0.5f, -0.5f,
//         0.5f, 0.5f, 0.5f,

//         -0.5f, 0.5f, -0.5f, // top face
//         -0.5f, 0.5f, 0.5f,
//         0.5f, 0.5f, 0.5f,
//         0.5f, 0.5f, 0.5f,
//         0.5f, 0.5f, -0.5f,
//         -0.5f, 0.5f, -0.5f,

//         -0.5f, -0.5f, -0.5f, // bottom face
//         0.5f, -0.5f, -0.5f,
//         0.5f, -0.5f, 0.5f,
//         0.5f, -0.5f, 0.5f,
//         -0.5f, -0.5f, 0.5f,
//         -0.5f, -0.5f, -0.5f};

//     void walkInASquare()
//     {
//         static int step = 0;           // Which side of the square
//         static int stepsTaken = 0;     // How far along that side we are
//         static int stepsPerSide = 100; // How many steps per side
//         static float speed = 0.05f;    // Speed per frame

//         switch (step)
//         {
//         case 0: // Move right
//             position.x += speed;
//             break;
//         case 1: // Move forward (increase z)
//             position.z += speed;
//             break;
//         case 2: // Move left
//             position.x -= speed;
//             break;
//         case 3: // Move backward (decrease z)
//             position.z -= speed;
//             break;
//         }

//         stepsTaken++;

//         if (stepsTaken >= stepsPerSide)
//         {
//             stepsTaken = 0;
//             step = (step + 1) % 4; // Loop through 0–3
//         }
//     }
// };
