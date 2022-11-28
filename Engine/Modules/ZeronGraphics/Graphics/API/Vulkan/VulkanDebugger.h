// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

#if ZE_GRAPHICS_VULKAN
#include <Graphics/API/Vulkan/VulkanCommon.h>

namespace Zeron
{
#if ZE_DEBUG
	class VulkanDebugger {
	public:
		static void Create(const vk::Instance& instance);
		static void Destroy(const vk::Instance& instance);
	private:
		static vk::DebugUtilsMessengerEXT mDebugMessenger;
	};
#endif
}
#endif
