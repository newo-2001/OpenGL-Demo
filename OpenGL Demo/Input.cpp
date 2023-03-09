#include "Input.h"

bool pressedKeys[1024] = { false };
EventTopic<Input::KeyEvent> keyEventTopic;
EventTopic<Input::MouseMoveEvent> mouseMoveEventTopic;

EventTopic<Input::KeyEvent>& Input::OnKeyEvent() { return keyEventTopic; }
EventTopic<Input::MouseMoveEvent>& Input::OnMouseMoveEvent() { return mouseMoveEventTopic; }

void Input::InitializeInputHandler()
{
    OnKeyEvent().Subscribe([] (const KeyEvent& event) -> void
    {
        if (event.action == GLFW_PRESS)
        {
            pressedKeys[event.key] = true;
        }
        else if (event.action == GLFW_RELEASE)
        {
            pressedKeys[event.key] = false;
        }
    });
}

bool Input::IsKeyPressed(int key)
{
    return pressedKeys[key];
}
