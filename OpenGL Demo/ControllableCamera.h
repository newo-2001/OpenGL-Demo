#pragma once

#include "Input.h"
#include "Camera.h"

class ControllableCamera : public Camera
{
public:
    ControllableCamera(glm::vec3 position = glm::vec3(), float yaw = 0.0f, float pitch = 0.0f);
    ~ControllableCamera();

    void SetMovementSpeed(float speed) { m_movementSpeed = speed; }
    float GetMovementSpeed() { return m_movementSpeed; }

    void Update();
    void OnMouseMoved(Input::MouseMoveEvent event);
private:
    float m_movementSpeed = 0.2f;
    float m_mouseSensitivity = 0.002f;
    
    long m_mouseMoveEventSubscription;
};
