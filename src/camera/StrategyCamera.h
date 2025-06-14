#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class StrategyCamera
{
public:
    StrategyCamera();

    void pan(float dx, float dz);
    void rotate(float dYaw);
    void tilt(float dPitch);
    void zoom(float delta);

    glm::mat4 getViewMatrix() const;
    glm::vec3 getPosition() const;

    void setTarget(const glm::vec3 &newTarget);
    glm::vec3 getTarget() const;

    float getPitch() const;
    void setPitch(float newPitch);

    glm::vec3 getFront() const
    {
        return glm::normalize(target - getPosition());
    }

private:
    glm::vec3 target;
    float distance;
    float yaw;
    float pitch;

    const float minPitch = glm::radians(10.0f);
    const float maxPitch = glm::radians(89.0f);
    const float minDistance = 5.0f;
    const float maxDistance = 100.0f;
};