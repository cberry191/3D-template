#define GLM_ENABLE_EXPERIMENTAL
#include <iostream>
#include "StrategyCamera.h"
#include <glm/gtx/string_cast.hpp>

StrategyCamera::StrategyCamera()
    : target(0.0f, 0.0f, 0.0f), distance(25.0f), yaw(glm::radians(45.0f)), pitch(glm::radians(45.0f)) {}

void StrategyCamera::pan(float dx, float dz)
{
    // Get forward direction, projected onto XZ plane
    glm::vec3 forward = glm::normalize(glm::vec3(target - getPosition()));
    forward.y = 0.0f;
    forward = glm::normalize(forward);

    // Right is perpendicular to forward in XZ
    glm::vec3 right = glm::normalize(glm::cross(glm::vec3(0, 1, 0), forward));

    // Movement remains on the ground
    glm::vec3 movement = right * dx + forward * dz;
    target += movement;
}

// void StrategyCamera::pan(float dx, float dz) // computes up based on tilt
// {
//     glm::vec3 forward = glm::normalize(target - getPosition());
//     glm::vec3 right = glm::normalize(glm::cross(forward, glm::vec3(0, 1, 0)));
//     glm::vec3 up = glm::normalize(glm::cross(right, forward));
//     glm::vec3 movement = right * dx + up * dz;
//     target += movement;
// }

void StrategyCamera::rotate(float dYaw)
{
    yaw += dYaw;
}

void StrategyCamera::tilt(float dPitch)
{
    pitch += dPitch;
    if (pitch < minPitch)
        pitch = minPitch;
    if (pitch > maxPitch)
        pitch = maxPitch;
}

void StrategyCamera::zoom(float delta)
{
    distance += delta;
    if (distance < minDistance)
        distance = minDistance;
    if (distance > maxDistance)
        distance = maxDistance;
}

glm::vec3 StrategyCamera::getPosition() const
{
    glm::vec3 offset;
    offset.x = distance * cos(pitch) * sin(yaw);
    offset.y = distance * sin(pitch);
    offset.z = distance * cos(pitch) * cos(yaw);
    return target + offset;
}

glm::mat4 StrategyCamera::getViewMatrix() const
{
    // std::cout << "Generating view matrix from pos=" << glm::to_string(getPosition())
    //   << " looking at target=" << glm::to_string(target) << std::endl;

    return glm::lookAt(getPosition(), target, glm::vec3(0, 1, 0));
}

void StrategyCamera::setTarget(const glm::vec3 &newTarget)
{
    target = newTarget;
}

glm::vec3 StrategyCamera::getTarget() const
{
    return target;
}

float StrategyCamera::getPitch() const
{
    return pitch;
}

void StrategyCamera::setPitch(float newPitch)
{
    pitch = glm::clamp(newPitch, minPitch, maxPitch);
}
