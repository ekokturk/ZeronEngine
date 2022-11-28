// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

#if ZE_WINDOW_GLFW

struct GLFWwindow;

namespace Zeron {

    class GLFWHelpers {
    public:
        static SystemHandle GetPlatformWindowHandle(GLFWwindow* glfwWindow);
    };
}

#endif