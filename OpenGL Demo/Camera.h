#pragma once

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

class Camera
{
public:
    Camera(glm::vec3 position = glm::vec3(), float yaw = 0.0f, float pitch = 0.0f);

    glm::vec3 GetPosition() const { return m_position; }
    glm::vec3 GetOrientation() const { return m_front; }

    float GetYaw() { return m_yaw; }
    float GetPitch() { return m_pitch; }

    void MoveTo(const glm::vec3& position) { m_position = position; }
    void Face(const glm::vec3& direction) { m_front = direction; }
    
    glm::mat4 GetViewMatrix() const;
    virtual void Update() {};

protected:
    glm::vec3 m_position, m_front, m_up, m_right;
    float m_yaw, m_pitch;

    void RecalculateBasisVectors();
};
