// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

#if ZE_GRAPHICS_VULKAN

#	include <Graphics/API/Vulkan/GraphicsAdapterVulkan.h>
#	include <Graphics/API/Vulkan/VulkanCommon.h>
#	include <Graphics/Graphics.h>
#	include <Graphics/GraphicsTypes.h>

namespace Zeron::Gfx
{
	class SwapChainVulkan;
	class CommandBufferVulkan;
	class TextureVulkan;
	class CommandBuffer;

	class GraphicsVulkan final : public Graphics {
	  public:
		GraphicsVulkan();
		~GraphicsVulkan() override;

		bool Init() override;

		GraphicsType GetGraphicsType() const override;
		MSAALevel GetMultiSamplingLevel() const override;

		std::unique_ptr<GraphicsContext> CreateGraphicsContext() override;
		std::unique_ptr<CommandBuffer> CreateCommandBuffer(uint32_t count, bool isCompute) override;

		std::unique_ptr<Pipeline> CreatePipelineGraphics(ShaderProgram* shader, RenderPass* renderPass, PipelineConfig config) override;
		std::unique_ptr<Pipeline> CreatePipelineCompute(ShaderProgram& shader) override;
		std::unique_ptr<PipelineBinding> CreatePipelineBinding(Pipeline& pipeline, const std::vector<BindingHandle>& bindingList) override;

		std::unique_ptr<RenderPass> CreateRenderPass(
			std::vector<RenderPassAttachment> colorAttachments, std::optional<RenderPassAttachment> depthAttachment, MSAALevel sampling
		) override;
		std::unique_ptr<FrameBuffer> CreateFrameBuffer(
			RenderPass& renderPass, const Vec2i& extent, const std::span<Texture*>& colorTextures, Texture* depthTextures, const std::span<Texture*>& resolveTextures
		) override;

		std::unique_ptr<Buffer> CreateBuffer(BufferType type, uint32_t count, uint32_t stride, const void* data, BufferUsageType usage) override;

		std::unique_ptr<ShaderProgram> CreateShaderProgram(const ShaderProgramConfig& config, std::unordered_map<ShaderType, ByteBuffer> shaderData) override;
		std::string GetCompiledShaderName(const std::string& shaderName, ShaderType type) const override;

		std::unique_ptr<Texture> CreateTexture(const Vec2i& size, TextureFormat format, const void* data, TextureType type, MSAALevel sampling) override;

		std::unique_ptr<Sampler> CreateSampler(SamplerAddressMode addressMode = SamplerAddressMode::Repeat, bool hasAnisotropicFilter = true) override;

	  public:
		// ---- Vulkan API
		std::unique_ptr<SwapChainVulkan> CreateSwapChainVK(SystemHandle systemHandle, const Vec2i& size);
		vk::UniqueCommandPool CreateCommandPoolVK() const;
		vk::UniqueDescriptorPool CreateDescriptorPoolVK(const std::vector<vk::DescriptorPoolSize>& poolSizeList) const;
		std::unique_ptr<TextureVulkan> CreateTextureVK(
			const Vec2i& size, TextureType type, TextureFormat format, const void* data, MSAALevel sampling, TextureLayout layout, vk::ImageUsageFlags usage
		);

		void SubmitSingleUseCommandBufferVK(std::function<void(CommandBufferVulkan&)> commands);

		vk::Instance& GetInstanceVK();
		vk::Device& GetDeviceVK();
		vk::PhysicalDevice& GetPrimaryAdapterVK();

		vk::Queue& GetGraphicsQueueVK();
		vk::Queue& GetPresentQueueVK();
		uint32_t GetGraphicsQueueFamilyIndexVK() const;
		uint32_t GetPresentQueueFamilyIndexVK() const;

		uint32_t GetMemoryTypeIndexVK(uint32_t filter, vk::MemoryPropertyFlags flags);

#	if ZE_DEBUG
		class VulkanObjectDebugInterface* getObjectDebugInterface() const;
#	endif

	  private:
		bool _initInstance();
		bool _initPrimaryAdapter();
		bool _initDevice(vk::SurfaceKHR surface);
		bool _verifyExtensions() const;
		void _initSupportedValidationLayers();

		MSAALevel _getMaxMultiSampleLevel();

		std::vector<GraphicsAdapterVulkan> _getGraphicsAdapters() const;

	  private:
		vk::ApplicationInfo mAppInfo;
		std::vector<std::string> mExtensions;
		std::vector<std::string> mValidationLayers;

		vk::Instance mInstance;
		std::unique_ptr<GraphicsAdapterVulkan> mAdapter;

		vk::Device mDevice;

		vk::Queue mGraphicsQueue;
		vk::Queue mPresentQueue;

		uint32_t mGraphicsQueueFamilyIndex;
		uint32_t mPresentQueueFamilyIndex;

		MSAALevel mMaxSupportedSampling = MSAALevel::Disabled;

#	if ZE_DEBUG
		std::unique_ptr<class VulkanObjectDebugInterface> mObjectDebugInterface;
#	endif
	};
}
#endif