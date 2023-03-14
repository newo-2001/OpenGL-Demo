#include <array>

#include "Input.h"

std::array<bool, 1024> pressedKeys = { false };
std::array<bool, 24> pressedButtons = { false };

EventTopic<Input::KeyEvent> OnKeyEvent;
EventTopic<Input::MouseMoveEvent> OnMouseMoveEvent;
EventTopic<Input::MouseButtonEvent> OnMouseButtonEvent;

void Input::InitializeInputHandler()
{
    OnKeyEvent.Subscribe([](KeyEvent event) -> void
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
    
    OnMouseButtonEvent.Subscribe([] (MouseButtonEvent event) -> void
    {
        if (event.action == GLFW_PRESS)
        {
            pressedButtons[event.button] = true;
        }
        else if (event.action == GLFW_RELEASE)
        {
            pressedButtons[event.button] = false;
        }
    });
}

bool Input::IsKeyPressed(int key)
{
    return pressedKeys[key];
}

bool Input::IsMouseButtonPressed(int button)
{
    return pressedButtons[button];
}

void Input::ReleaseAll()
{
    pressedButtons.fill(false);
    pressedKeys.fill(false);
}
