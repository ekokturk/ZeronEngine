// Copyright (C) Eser Kokturk. All Rights Reserved.

#if ZE_WINDOW_GLFW
#include <Window/API/GLFW/GLFWHelpers.h>

#if ZE_PLATFORM_WIN32
	#define GLFW_EXPOSE_NATIVE_WIN32
#elif ZE_PLATFORM_LINUX
	#define GLFW_EXPOSE_NATIVE_X11
#endif

#include <GLFW/glfw3.h>
#include "GLFW/glfw3native.h"

namespace Zeron {

    SystemHandle GLFWHelpers::GetPlatformWindowHandle(GLFWwindow* glfwWindow) {
		SystemHandle handle;
    	#if ZE_PLATFORM_WIN32
			handle.mWindow = glfwGetWin32Window(glfwWindow);
		#elif ZE_PLATFORM_LINUX
			handle.mWindow = glfwGetX11Window(glfwWindow); 
			handle.mConnection = glfwGetX11Display(); 
		#endif
		return handle;
    }

}

#endif
