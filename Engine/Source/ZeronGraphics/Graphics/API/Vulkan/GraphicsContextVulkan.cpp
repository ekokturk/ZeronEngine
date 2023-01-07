// Copyright (C) Eser Kokturk. All Rights Reserved.

#ifdef ZE_GRAPHICS_VULKAN
#include <Graphics/API/Vulkan/GraphicsContextVulkan.h>

#include <Graphics/API/Vulkan/CommandBufferVulkan.h>
#include <Graphics/API/Vulkan/FrameBufferVulkan.h>
#include <Graphics/API/Vulkan/GraphicsVulkan.h>
#include <Graphics/API/Vulkan/SwapChainVulkan.h>
#include <Graphics/API/Vulkan/TextureVulkan.h>


namespace Zeron {
	
	GraphicsContextVulkan::GraphicsContextVulkan(GraphicsVulkan& graphics)
		: mGraphics(graphics)
		, mMaxFramesInFlight(2)
		, mCurrentFrameInFlight(0)
	{
	}

	GraphicsContextVulkan::~GraphicsContextVulkan()
	{
	}

	void GraphicsContextVulkan::Init(SystemHandle systemHandle, const Vec2i& size)
	{
		ZE_ASSERT(!mSwapChain, "Vulkan graphics context swapchain was already initialized!");
		mSwapChain = mGraphics.CreateSwapChainVK(systemHandle, size);

		const vk::Device& device = mGraphics.GetDeviceVK();
		for (uint32_t i = 0; i < mMaxFramesInFlight; ++i) {
			mRenderCompleteSemaphores.push_back(device.createSemaphoreUnique(vk::SemaphoreCreateInfo()));
			mPresentReadySemaphores.push_back(device.createSemaphoreUnique(vk::SemaphoreCreateInfo()));
			mGraphicsFences.push_back(device.createFenceUnique(vk::FenceCreateInfo{ vk::FenceCreateFlagBits::eSignaled }));
		}
		mCommandBuffer = std::make_unique<CommandBufferVulkan>(mGraphics, mSwapChain->GetFrameCount(), false);
	}

	CommandBuffer& GraphicsContextVulkan::BeginCommands()
	{
		_acquireNextSwapChainFrame();

		mCommandBuffer->Begin();
		return *mCommandBuffer;
	}

	void GraphicsContextVulkan::EndCommands()
	{
		mCommandBuffer->End();
	}

	void GraphicsContextVulkan::BeginSwapChainRenderPass(CommandBuffer& cmd) const
	{
		cmd.BeginRenderPass(mSwapChain->GetFrameBuffer(), &mSwapChain->GetRenderPass());
	}

	void GraphicsContextVulkan::EndSwapChainRenderPass(CommandBuffer& cmd) const
	{
		cmd.EndRenderPass();
	}

	void GraphicsContextVulkan::Submit(CommandBuffer& cmd)
	{
		const vk::Device& device = mGraphics.GetDeviceVK();
		const vk::Fence& fence = _getCurrentGraphicsFence();

		auto& vkCmdBuffer = static_cast<CommandBufferVulkan&>(cmd);
		const vk::PipelineStageFlags pipelineStage { vk::PipelineStageFlagBits::eColorAttachmentOutput };

		const vk::SubmitInfo submitInfo(
			1,
			&_getCurrentPresentReadySemaphore(),
			&pipelineStage,
			1,
			&vkCmdBuffer.GetCommandBufferVK(),
			1,
			&_getCurrentRenderCompleteSemaphore()
		);
		ZE_VK_ASSERT(mGraphics.GetGraphicsQueueVK().submit(1, &submitInfo, fence), "Vulkan command buffer submission failed!");
		ZE_VK_ASSERT(device.waitForFences(1, &fence, VK_TRUE, ZE_VK_TIMEOUT), "Vulkan failed to wait for fences!");
	}

	void GraphicsContextVulkan::Present()
	{
		mSwapChain->Present(mGraphics, _getCurrentRenderCompleteSemaphore());
		mCurrentFrameInFlight = (mCurrentFrameInFlight + 1) % mMaxFramesInFlight;
	}

	void GraphicsContextVulkan::ResizeSwapChain(const Vec2i& size)
	{
		mSwapChain->Recreate(mGraphics, size);
	}

	SwapChain* GraphicsContextVulkan::GetSwapChain() const
	{
		return mSwapChain.get();
	}

	RenderPass* GraphicsContextVulkan::GetSwapChainRenderPass() const
	{
		ZE_ASSERT(mSwapChain, "Swap chain doesn't exist to retrieve render pass!");
		return &mSwapChain->GetRenderPass();
	}

	const Vec2i& GraphicsContextVulkan::GetSwapChainSize() const
	{
		return mSwapChain->GetSize();
	}

	uint32_t GraphicsContextVulkan::GetMaxFramesInFlight() const
	{
		return mMaxFramesInFlight;
	}

	vk::Semaphore& GraphicsContextVulkan::_getCurrentPresentReadySemaphore()
	{
		ZE_ASSERT(mPresentReadySemaphores.size() > mCurrentFrameInFlight);
		return mPresentReadySemaphores[mCurrentFrameInFlight].get();
	}

	vk::Semaphore& GraphicsContextVulkan::_getCurrentRenderCompleteSemaphore()
	{
		ZE_ASSERT(mRenderCompleteSemaphores.size() > mCurrentFrameInFlight);
		return mRenderCompleteSemaphores[mCurrentFrameInFlight].get();
	}

	vk::Fence& GraphicsContextVulkan::_getCurrentGraphicsFence()
	{
		ZE_ASSERT(mGraphicsFences.size() > mCurrentFrameInFlight);
		return mGraphicsFences[mCurrentFrameInFlight].get();
	}

	void GraphicsContextVulkan::_acquireNextSwapChainFrame()
	{
		const vk::Device& device = mGraphics.GetDeviceVK();
		const vk::Fence& fence = _getCurrentGraphicsFence();
		mSwapChain->AcquireNextFrame(device, _getCurrentPresentReadySemaphore());
		ZE_VK_ASSERT(device.resetFences(1, &fence), "Vulkan failed to reset fences!");
	}
}
#endif
