// Copyright (C) Eser Kokturk. All Rights Reserved.

#include <Graphics/API/Vulkan/VulkanHelpers.h>
#if ZE_GRAPHICS_VULKAN
#include <Graphics/API/Vulkan/SwapChainVulkan.h>

#include <Graphics/API/Vulkan/FrameBufferVulkan.h>
#include <Graphics/API/Vulkan/GraphicsVulkan.h>
#include <Graphics/API/Vulkan/RenderPassVulkan.h>
#include <Graphics/API/Vulkan/TextureVulkan.h>

namespace Zeron
{
	SwapChainVulkan::SwapChainVulkan(GraphicsVulkan& graphics, SystemHandle systemHandle, const Vec2i& size, vk::UniqueSurfaceKHR surface)
		: SwapChain(size, 3)
		, mSurface(std::move(surface))
		, mSystemHandle(systemHandle)
		, mCurrentFrameIndex(0)
		, mColorFormat(vk::Format::eB8G8R8A8Unorm)
		, mColorSpace(vk::ColorSpaceKHR::eSrgbNonlinear)
		, mDepthFormat(vk::Format::eD32Sfloat)
		, mPresentMode(vk::PresentModeKHR::eFifo)
	{
		const vk::PhysicalDevice& physicalDevice = graphics.GetPrimaryAdapterVK();

		const std::vector<vk::SurfaceFormatKHR> formats = physicalDevice.getSurfaceFormatsKHR(*mSurface);
		ZE_ASSERT(std::find(formats.begin(), formats.end(), mColorFormat) != formats.end(), "Vulkan couldn't find compatible surface format for swap chain!");

		std::vector<vk::PresentModeKHR> availableModes = physicalDevice.getSurfacePresentModesKHR(*mSurface);
		ZE_ASSERT(std::find(availableModes.begin(), availableModes.end(), mPresentMode) != availableModes.end(), "Vulkan swap chain presentation mode is not supported!");

		const vk::SurfaceCapabilitiesKHR surfaceCapabilities = physicalDevice.getSurfaceCapabilitiesKHR(*mSurface);
		mSurfaceTransform = (surfaceCapabilities.supportedTransforms & vk::SurfaceTransformFlagBitsKHR::eIdentity) ? vk::SurfaceTransformFlagBitsKHR::eIdentity : surfaceCapabilities.currentTransform;
		mCompositeAlpha = (surfaceCapabilities.supportedCompositeAlpha & vk::CompositeAlphaFlagBitsKHR::ePreMultiplied) ? vk::CompositeAlphaFlagBitsKHR::ePreMultiplied
			: (surfaceCapabilities.supportedCompositeAlpha & vk::CompositeAlphaFlagBitsKHR::ePostMultiplied) ? vk::CompositeAlphaFlagBitsKHR::ePostMultiplied
			: (surfaceCapabilities.supportedCompositeAlpha & vk::CompositeAlphaFlagBitsKHR::eInherit) ? vk::CompositeAlphaFlagBitsKHR::eInherit
			: vk::CompositeAlphaFlagBitsKHR::eOpaque;

		ZE_ASSERT(GetFrameCount() >= surfaceCapabilities.minImageCount, "Vulkan swap chain must have minimun {} frames!", surfaceCapabilities.minImageCount);
		if(surfaceCapabilities.maxImageCount != 0) {
			ZE_ASSERT(GetFrameCount() <= surfaceCapabilities.maxImageCount, "Vulkan swap chain must have maximum {} frames!", surfaceCapabilities.maxImageCount);
		}

		mSwapChainRenderPass = std::make_unique<RenderPassVulkan>(graphics, mColorFormat, mDepthFormat, VulkanHelpers::GetMultiSamplingLevel(graphics.GetMultiSamplingLevel()));
		mPreDepthRenderPass = std::make_unique<RenderPassVulkan>(graphics, vk::Format::eUndefined, mDepthFormat, VulkanHelpers::GetMultiSamplingLevel(graphics.GetMultiSamplingLevel()));

		_createSwapChain(graphics, nullptr);
		_createFrameBuffers(graphics);
	}

	SwapChainVulkan::~SwapChainVulkan()
	{
	}

	void SwapChainVulkan::Present()
	{
		// We handle presentation through Render Context
	}

	void SwapChainVulkan::SetVSyncEnabled(bool isEnabled)
	{
	}

	void SwapChainVulkan::SetRefreshRate(uint16_t rate)
	{
	}

	bool SwapChainVulkan::IsVSyncEnabled() const
	{
		return false;
	}

	FrameBuffer* SwapChainVulkan::GetFrameBuffer() const
	{
		ZE_ASSERT(GetFrameCount() > mCurrentFrameIndex);
		return mSwapChainFrameBuffers[mCurrentFrameIndex].get();
	}

	RenderPass& SwapChainVulkan::GetRenderPass()
	{
		ZE_ASSERT(mSwapChainRenderPass, "Vulkan swap chain render pass cannot be null on request!");
		return *mSwapChainRenderPass;
	}

	uint32_t SwapChainVulkan::GetCurrentFrameIndex() const
	{
		return mCurrentFrameIndex;
	}

	vk::SwapchainKHR& SwapChainVulkan::GetSwapChainVK()
	{
		return *mSwapChain;
	}

	vk::Format SwapChainVulkan::GetColorFormatVK() const
	{
		return mColorFormat;
	}

	vk::Extent2D SwapChainVulkan::GetExtendVK() const
	{
		const Vec2i& size = GetSize();
		return vk::Extent2D { static_cast<uint32_t>(size.X), static_cast<uint32_t>(size.Y) };
	}

	void SwapChainVulkan::AcquireNextFrame(const vk::Device& device, vk::Semaphore semaphore)
	{
		const vk::ResultValue nextImageResult = device.acquireNextImageKHR(*mSwapChain, ZE_VK_TIMEOUT, semaphore, nullptr);
		ZE_VK_ASSERT(nextImageResult.result, "Unable to acquire next Vulkan swapchain image!");
		mCurrentFrameIndex = nextImageResult.value;
	}

	void SwapChainVulkan::Recreate(GraphicsVulkan& graphics, const Vec2i& size)
	{
		graphics.GetDeviceVK().waitIdle();
		_setSize(size);
		_createSwapChain(graphics, *mSwapChain);
		_createFrameBuffers(graphics);
	}

	void SwapChainVulkan::Present(GraphicsVulkan& graphics, vk::Semaphore semaphore)
	{
		const vk::PresentInfoKHR presentInfo(
			1,
			&semaphore,
			1,
			&mSwapChain.get(),
			&mCurrentFrameIndex,
			nullptr
		);

		const vk::Result presentResult = graphics.GetPresentQueueVK().presentKHR(&presentInfo);
		if (presentResult == vk::Result::eSuboptimalKHR || presentResult == vk::Result::eErrorOutOfDateKHR) {
			// Recreate swap chain
			Recreate(graphics, GetSize());
		}
		else if (presentResult != vk::Result::eSuccess) {
			ZE_VK_ASSERT(presentResult, "Unable to present Vulkan swap chain image!");
		}

	}

	void SwapChainVulkan::_createSwapChain(GraphicsVulkan& graphics, vk::SwapchainKHR oldSwapChain)
	{
		if (oldSwapChain) {
			mColorTextures.clear();
			mDepthTexture = nullptr;
			mSamplingTexture = nullptr;
			mSwapChainFrameBuffers.clear();
			mPreDepthFrameBuffer = nullptr;
		}

		const vk::SurfaceCapabilitiesKHR surfaceCapabilities = graphics.GetPrimaryAdapterVK().getSurfaceCapabilitiesKHR(*mSurface);
		ZE_ASSERT(surfaceCapabilities.currentExtent.width != 0, "Vulkan swap chain surface cannot have 0 width!");
		ZE_ASSERT(surfaceCapabilities.currentExtent.height != 0, "Vulkan swap chain surface cannot have 0 height!");

		vk::SwapchainCreateInfoKHR swapChainCreateInfo(
			vk::SwapchainCreateFlagsKHR(),
			*mSurface,
			GetFrameCount(),
			mColorFormat,
			mColorSpace,
			GetExtendVK(),
			1,
			vk::ImageUsageFlagBits::eColorAttachment,
			vk::SharingMode::eExclusive,
			{},
			mSurfaceTransform,
			mCompositeAlpha,
			mPresentMode,
			true,
			oldSwapChain
		);

		// We need to do this for discrete presentation queues so images can be shared between both queues
		const uint32_t graphicsQueueIndex = graphics.GetGraphicsQueueFamilyIndexVK();
		const uint32_t presentQueueIndex = graphics.GetPresentQueueFamilyIndexVK();
		if (presentQueueIndex != graphicsQueueIndex) {
			const std::array<uint32_t, 2> queueIndices{ graphicsQueueIndex, presentQueueIndex };
			swapChainCreateInfo.queueFamilyIndexCount = 2;
			swapChainCreateInfo.imageSharingMode = vk::SharingMode::eConcurrent;
			swapChainCreateInfo.pQueueFamilyIndices = queueIndices.data();
		}

		mSwapChain = graphics.GetDeviceVK().createSwapchainKHRUnique(swapChainCreateInfo);
	}

	void SwapChainVulkan::_createFrameBuffers(GraphicsVulkan& graphics)
	{
		const vk::SampleCountFlagBits sampling = VulkanHelpers::GetMultiSamplingLevel(graphics.GetMultiSamplingLevel());
		for (const vk::Image colorTexture : graphics.GetDeviceVK().getSwapchainImagesKHR(*mSwapChain)) {
			mColorTextures.emplace_back(TextureVulkan(colorTexture, GetSize(), mColorFormat, sampling));
		}

		// Create shared image wrappers
		mDepthTexture = graphics.CreateTextureVK(GetSize(), mDepthFormat, vk::ImageTiling::eOptimal, vk::ImageUsageFlagBits::eDepthStencilAttachment, 
			sampling, vk::ImageLayout::eUndefined, vk::ImageLayout::eDepthStencilAttachmentOptimal);

		if(sampling != vk::SampleCountFlagBits::e1) {
			mSamplingTexture = graphics.CreateTextureVK(GetSize(), mColorFormat, vk::ImageTiling::eOptimal, vk::ImageUsageFlagBits::eTransientAttachment | vk::ImageUsageFlagBits::eColorAttachment, 
				sampling, vk::ImageLayout::eUndefined, vk::ImageLayout::eColorAttachmentOptimal);
		}

		for (TextureVulkan& colorTexture : mColorTextures) {
			mSwapChainFrameBuffers.emplace_back(std::make_unique<FrameBufferVulkan>(graphics, GetExtendVK(), *mSwapChainRenderPass, &colorTexture, mDepthTexture.get(), mSamplingTexture.get()));
		}

		mPreDepthFrameBuffer = std::make_unique<FrameBufferVulkan>(graphics, GetExtendVK(), *mPreDepthRenderPass, nullptr, mDepthTexture.get(), nullptr);
	}
}
#endif
