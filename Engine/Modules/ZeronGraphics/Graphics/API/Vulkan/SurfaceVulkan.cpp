// Copyright (C) Eser Kokturk. All Rights Reserved.

#if ZE_GRAPHICS_VULKAN

#if ZE_PLATFORM_WIN32
	#define VK_USE_PLATFORM_WIN32_KHR
#elif ZE_PLATFORM_LINUX
	#define VK_USE_PLATFORM_XLIB_KHR
#elif ZE_PLATFORM_ANDROID
	#define VK_USE_PLATFORM_ANDROID_KHR
#endif

#include <Graphics/API/Vulkan/SurfaceVulkan.h>

namespace Zeron
{
	vk::UniqueSurfaceKHR SurfaceVulkan::CreateSurfaceVK(const vk::Instance& instance, SystemHandle systemHandle)
	{
		VkSurfaceKHR surfaceTemp;
#if ZE_PLATFORM_WIN32
		const VkWin32SurfaceCreateInfoKHR surfaceInfo = {
			VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR,              
			nullptr,               
			0,
			GetModuleHandle(nullptr),            
			static_cast<HWND>(systemHandle.mWindow)
		};
		const auto vkCreateWin32SurfaceKHR = reinterpret_cast<PFN_vkCreateWin32SurfaceKHR>(instance.getProcAddr("vkCreateWin32SurfaceKHR"));
		vkCreateWin32SurfaceKHR(instance, &surfaceInfo, nullptr, &surfaceTemp);
#elif ZE_PLATFORM_LINUX
		const VkXlibSurfaceCreateInfoKHR surfaceInfo = {
			VK_STRUCTURE_TYPE_XLIB_SURFACE_CREATE_INFO_KHR,
			nullptr,
			0,
			reinterpret_cast<Display*>(systemHandle.mConnection),
			systemHandle.mWindow
		};
		const auto vkCreateXlibSurfaceKHR = reinterpret_cast<PFN_vkCreateXlibSurfaceKHR>(instance.getProcAddr("vkCreateXlibSurfaceKHR"));
		vkCreateXlibSurfaceKHR(instance, &surfaceInfo, nullptr, &surfaceTemp);
#elif ZE_PLATFORM_ANDROID
		const VkAndroidSurfaceCreateInfoKHR surfaceInfo = {
			VK_STRUCTURE_TYPE_ANDROID_SURFACE_CREATE_INFO_KHR,
			nullptr,
			0,
			reinterpret_cast<ANativeWindow*>(systemHandle.mWindow)
		};
		const auto vkCreateAndroidSurfaceKHR = reinterpret_cast<PFN_vkCreateAndroidSurfaceKHR>(instance.getProcAddr("vkCreateAndroidSurfaceKHR"));
		vkCreateAndroidSurfaceKHR(instance, &surfaceInfo, nullptr, &surfaceTemp);
#else
		ZE_FAIL("Platform is not supported for Vulkan!");
#endif
		// We need to pass in a custom deleter to use unique handle
		const vk::ObjectDestroy<vk::Instance, VULKAN_HPP_DEFAULT_DISPATCHER_TYPE> surfaceDeleter(instance);
		return vk::UniqueSurfaceKHR(surfaceTemp, surfaceDeleter);
	}
}

#endif
