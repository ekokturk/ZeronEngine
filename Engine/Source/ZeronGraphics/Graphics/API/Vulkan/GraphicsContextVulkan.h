// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

#if ZE_GRAPHICS_VULKAN
#include <Graphics/GraphicsContext.h>

#include <Graphics/API/Vulkan/VulkanCommon.h>

namespace Zeron
{
	class RenderPass;
	class CommandBufferVulkan;
	class SwapChainVulkan;
	class TextureVulkan;
	class CommandBuffer;
	class FrameBufferVulkan;
	class GraphicsVulkan;

	class GraphicsContextVulkan final : public GraphicsContext {
	public:
		GraphicsContextVulkan(GraphicsVulkan& graphics);
		~GraphicsContextVulkan();

		void Init(SystemHandle systemHandle, const Vec2i& size) override;
		CommandBuffer& BeginCommands() override;
		void EndCommands() override;

		void Submit(CommandBuffer& cmd) override;
		void BeginSwapChainRenderPass(CommandBuffer& cmd) const override;
		void EndSwapChainRenderPass(CommandBuffer& cmd) const override;

		void Present() override;
		void ResizeSwapChain(const Vec2i& size) override;

		SwapChain* GetSwapChain() const override;
		RenderPass* GetSwapChainRenderPass() const override;
		const Vec2i& GetSwapChainSize() const override;

		uint32_t GetMaxFramesInFlight() const override;

	private:
		vk::Semaphore& _getCurrentRenderCompleteSemaphore();
		vk::Semaphore& _getCurrentPresentReadySemaphore();
		vk::Fence& _getCurrentGraphicsFence();
		void _acquireNextSwapChainFrame();

		GraphicsVulkan& mGraphics;

		std::vector<vk::UniqueSemaphore> mRenderCompleteSemaphores;
		std::vector<vk::UniqueSemaphore> mPresentReadySemaphores;
		std::vector<vk::UniqueFence> mGraphicsFences;

		std::unique_ptr<SwapChainVulkan> mSwapChain;
		std::unique_ptr<CommandBufferVulkan> mCommandBuffer;

		const uint32_t mMaxFramesInFlight;
		uint32_t mCurrentFrameInFlight;
	};
}
#endif
