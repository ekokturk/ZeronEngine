// Copyright (C) Eser Kokturk. All Rights Reserved.

#if ZE_GRAPHICS_VULKAN

#	include <Graphics/API/Vulkan/VulkanDebug.h>
#	include <Graphics/API/Vulkan/VulkanInstance.h>

namespace Zeron::Gfx
{
	vk::Instance VulkanInstance::mInstance;
	uint32_t VulkanInstance::mRefCount = 0;

#	if (VULKAN_HPP_DISPATCH_LOADER_DYNAMIC == 1)

	VulkanInstance::DynamicLoader VulkanInstance::mDynamicLoader;
#	endif

#	if ZE_DEBUG
	vk::DebugUtilsMessengerEXT VulkanInstance::mDebugMessenger;
#	endif

	void VulkanInstance::LoadProc()
	{
#	if (VULKAN_HPP_DISPATCH_LOADER_DYNAMIC == 1)
		ZE_ASSERT(mDynamicLoader.IsSuccess(), "Unable to find Vulkan library on this platform");
		auto vkGetInstanceProcAddr = mDynamicLoader.GetInstanceProcAddress();
		VULKAN_HPP_DEFAULT_DISPATCHER.init(vkGetInstanceProcAddr);
#	endif
	}

	vk::Instance VulkanInstance::CreateRef(vk::InstanceCreateInfo createInfo)
	{
		if (mRefCount == 0) {
			ZE_ASSERT(!mInstance, "Vulkan instance shouldn't exist with not references");
			mInstance = vk::createInstance(createInfo);

#	if (VULKAN_HPP_DISPATCH_LOADER_DYNAMIC == 1)
			VULKAN_HPP_DEFAULT_DISPATCHER.init(mInstance);
#	endif

#	if ZE_DEBUG
			mDebugMessenger = CreateVulkanDebugUtilsMessenger(mInstance);
#	endif
		}
		++mRefCount;
		return mInstance;
	}

	void VulkanInstance::DestroyRef()
	{
		if (mRefCount != 0) {
			--mRefCount;
		}
		if (mRefCount == 0 && mInstance) {
#	if ZE_DEBUG
			mInstance.destroyDebugUtilsMessengerEXT(mDebugMessenger);
			mDebugMessenger = nullptr;
#	endif
			mInstance.destroy();
			mInstance = nullptr;
		}
	}

	VulkanInstance::DynamicLoader::DynamicLoader()
		: mSuccess(false)
	{
#	if ZE_PLATFORM_ANDROID || ZE_PLATFORM_LINUX
		mLibrary = dlopen("libvulkan.so", RTLD_NOW | RTLD_LOCAL);
		if (mLibrary == 0) {
			mLibrary = dlopen("libvulkan.so.1", RTLD_NOW | RTLD_LOCAL);
		}
#	elif ZE_PLATFORM_WIN32
		mLibrary = LoadLibrary("vulkan-1.dll");
#	endif

		mSuccess = mLibrary != 0;
	}

	VulkanInstance::DynamicLoader::~DynamicLoader()
	{
		if (mLibrary) {
#	if ZE_PLATFORM_ANDROID || ZE_PLATFORM_LINUX
			dlclose(mLibrary);
#	elif ZE_PLATFORM_WIN32
			FreeLibrary(static_cast<HMODULE>(mLibrary));
#	endif
		}
	}

	PFN_vkGetInstanceProcAddr VulkanInstance::DynamicLoader::GetInstanceProcAddress() const
	{
#	if ZE_PLATFORM_ANDROID || ZE_PLATFORM_LINUX
		return reinterpret_cast<PFN_vkGetInstanceProcAddr>(dlsym(mLibrary, "vkGetInstanceProcAddr"));
#	elif ZE_PLATFORM_WIN32
		return reinterpret_cast<PFN_vkGetInstanceProcAddr>(GetProcAddress(static_cast<HMODULE>(mLibrary), "vkGetInstanceProcAddr"));
#	endif
	}
}
#endif