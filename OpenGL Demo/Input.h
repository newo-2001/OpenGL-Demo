#pragma once

#include "GLFW/glfw3.h"
#include <functional>

#include "EventTopic.h"

namespace Input
{
    struct KeyEvent
    {
        int key, action;
    };

    struct MouseButtonEvent
    {
        int button, action;
    };

    struct MouseMoveEvent
    {
        float dx, dy;
    };
    
    bool IsKeyPressed(int key);
    bool IsMouseButtonPressed(int button);

    void InitializeInputHandler();
    void ReleaseAll();

    inline EventTopic<KeyEvent> OnKeyEvent;
    inline EventTopic<MouseButtonEvent> OnMouseButtonEvent;
    inline EventTopic<MouseMoveEvent> OnMouseMoveEvent;
}
