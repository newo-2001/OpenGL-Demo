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

    struct MouseMoveEvent
    {
        float dx, dy;
    };
    
    bool IsKeyPressed(int key);
    void InitializeInputHandler();

    EventTopic<KeyEvent>& OnKeyEvent();
    EventTopic<MouseMoveEvent>& OnMouseMoveEvent();
}
