#include <numbers>

#include "ControllableCamera.h"
#include "Input.h"

#include <GLFW/glfw3.h>
#include <glm/ext/matrix_transform.hpp>

const glm::vec3 WORLD_UP = glm::vec3(0.0, 1.0f, 0.0f);

ControllableCamera::ControllableCamera(glm::vec3 position, float yaw, float pitch) : Camera(position, yaw, pitch)
{
    m_mouseMoveEventSubscription = Input::OnMouseMoveEvent().Subscribe([this] (Input::MouseMoveEvent event) -> void
    {
        this->OnMouseMoved(event);
    });
}

ControllableCamera::~ControllableCamera()
{
    Input::OnMouseMoveEvent().Unsubscribe(m_mouseMoveEventSubscription);
}

void ControllableCamera::Update()
{
    if (Input::IsKeyPressed(GLFW_KEY_W))
    {
        m_position += m_front * glm::vec3(m_movementSpeed, 0.0f, m_movementSpeed);
    }
    
    if (Input::IsKeyPressed(GLFW_KEY_S))
    {
        m_position -= m_front * glm::vec3(m_movementSpeed, 0.0f, m_movementSpeed);
    }

    if (Input::IsKeyPressed(GLFW_KEY_A))
    {
        m_position -= m_right * glm::vec3(m_movementSpeed, 0.0f, m_movementSpeed);
    }

    if (Input::IsKeyPressed(GLFW_KEY_D))
    {
        m_position += m_right * glm::vec3(m_movementSpeed, 0.0f, m_movementSpeed);
    }

    if (Input::IsKeyPressed(GLFW_KEY_SPACE))
    {
        m_position += WORLD_UP * m_movementSpeed;
    }

    if (Input::IsKeyPressed(GLFW_KEY_LEFT_SHIFT))
    {
        m_position -= WORLD_UP * m_movementSpeed;
    }
}

void ControllableCamera::OnMouseMoved(Input::MouseMoveEvent event)
{
    constexpr float MAX_ANGLE = std::numbers::pi - 0.05f;
    constexpr float MIN_ANGLE = -MAX_ANGLE;
    
    m_yaw = fmod(m_yaw + event.dx * m_mouseSensitivity, 360.0f);
    m_pitch = glm::clamp(m_pitch - event.dy * m_mouseSensitivity, MIN_ANGLE, MAX_ANGLE);

    Camera::RecalculateBasisVectors();
}
