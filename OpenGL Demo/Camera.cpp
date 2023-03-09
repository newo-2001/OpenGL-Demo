#include <glm/ext/matrix_transform.hpp>

#include "Camera.h"

const glm::vec3 WORLD_UP = { 0.0f, 1.0f, 0.0f };

Camera::Camera(glm::vec3 position, float yaw, float pitch) :
    m_position(position),
    m_yaw(yaw),
    m_pitch(pitch)
{
    RecalculateBasisVectors();
}

glm::mat4 Camera::GetViewMatrix() const
{
    return glm::lookAt(m_position, m_position + m_front, m_up);
}

void Camera::RecalculateBasisVectors()
{
    m_front.x = cos(m_yaw) * cos(m_pitch);
    m_front.y = sin(m_pitch);
    m_front.z = sin(m_yaw) * cos(m_pitch);
    m_front = glm::normalize(m_front);
        
    m_right = glm::normalize(glm::cross(m_front, WORLD_UP));
    m_up = glm::normalize(glm::cross(m_right, m_front));
}
