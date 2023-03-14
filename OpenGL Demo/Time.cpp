#include <GLFW/glfw3.h>
#include <iostream>

#include "Time.h"

const unsigned int TPS = 30;

double lastFrameTime = glfwGetTime();
float delta = 1.0f;

namespace Time
{
    const int TPS = 30;

    void Update()
    {
        double time = glfwGetTime();

        double diff = time - lastFrameTime;
        lastFrameTime = time;
        
        delta = (float) (TPS / (1.0f / diff));
    }

    unsigned int GetFps()
    {
        return (unsigned int) (TPS / delta);
    }

    float GetDelta()
    {
        return delta;
    }
}