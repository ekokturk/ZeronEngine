// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

#if ZE_WINDOW_GLFW

#include <Input/KeyCode.h>
#include <Input/MouseCode.h>

struct GLFWwindow;

namespace Zeron {

    class GLFWHelpers {
    public:
        static SystemHandle GetPlatformWindowHandle(GLFWwindow* glfwWindow);
        static KeyCode GetKeyCode(int code);
        static MouseCode GetMouseCode(int code);
    };
}

#endif