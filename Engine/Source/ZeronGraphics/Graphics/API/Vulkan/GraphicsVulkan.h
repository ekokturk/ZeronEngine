// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

#if ZE_GRAPHICS_VULKAN

#	include <Graphics/API/Vulkan/GraphicsAdapterVulkan.h>
#	include <Graphics/API/Vulkan/VulkanCommon.h>
#	include <Graphics/Graphics.h>
#	include <Graphics/GraphicsTypes.h>

namespace Zeron
{
	class SwapChainVulkan;
	class CommandBufferVulkan;
	class TextureVulkan;
	class CommandBuffer;

	class GraphicsVulkan final : public Graphics {
	  public:
		GraphicsVulkan();
		~GraphicsVulkan();

		bool Init() override;

		GraphicsType GetGraphicsType() const override;
		MSAALevel GetMultiSamplingLevel() const override;

		std::unique_ptr<GraphicsContext> CreateGraphicsContext() override;
		std::unique_ptr<CommandBuffer> CreateCommandBuffer(uint32_t count, bool isCompute) override;

		std::unique_ptr<Pipeline> CreatePipeline(ShaderProgram* shader) override;
		std::unique_ptr<Pipeline> CreatePipeline(
			ShaderProgram* shader, RenderPass* renderPass, MSAALevel samplingLevel, PrimitiveTopology topology, bool isSolidFill, FaceCullMode cullMode
		) override;
		std::unique_ptr<PipelineBinding> CreatePipelineBinding(Pipeline& pipeline, const std::vector<BindingHandle>& bindingList) override;

		std::unique_ptr<Buffer> CreateBuffer(BufferType type, uint32_t count, uint32_t stride, const void* data, BufferUsageType usage = BufferUsageType::Default) override;

		std::unique_ptr<ShaderProgram> CreateShaderProgram(
			const std::string& shaderName, const VertexLayout& vertexLayout, const ResourceLayout& resourceLayout, const ByteBuffer& vertexShader = {},
			const ByteBuffer& fragmentShader = {}, const ByteBuffer& computeShader = {}
		) override;
		std::unique_ptr<ShaderProgram> CreateShaderProgram(
			const std::string& shaderName, const std::shared_ptr<Shader>& vertexShader, const std::shared_ptr<Shader>& fragmentShader, const VertexLayout& vertexLayout,
			const ResourceLayout& resourceLayout
		) override;
		std::string GetCompiledShaderName(const std::string& shaderName, ShaderType type) const override;

		std::unique_ptr<Texture> CreateTexture(TextureType type, const Color& data) override;
		std::unique_ptr<Texture> CreateTexture(TextureType type, const Color* data, uint32_t width, uint32_t height) override;

		std::unique_ptr<Sampler> CreateSampler(SamplerAddressMode addressMode = SamplerAddressMode::Repeat, bool hasAnisotropicFilter = true) override;

	  public:
		// ---- Vulkan API
		std::unique_ptr<SwapChainVulkan> CreateSwapChainVK(SystemHandle systemHandle, const Vec2i& size);
		vk::UniqueCommandPool CreateCommandPoolVK(bool useComputeQueue) const;
		vk::UniqueDescriptorPool CreateDescriptorPoolVK(const std::vector<vk::DescriptorPoolSize>& poolSizeList) const;
		std::unique_ptr<TextureVulkan> CreateTextureVK(
			const Vec2i& size, vk::Format format, vk::ImageTiling tiling, vk::ImageUsageFlags usage, vk::SampleCountFlagBits sampling, vk::ImageLayout oldLayout,
			vk::ImageLayout newLayout, const Color* data = nullptr
		);

		void SubmitSingleUseCommandBufferVK(std::function<void(CommandBufferVulkan&)> commands);

		vk::Instance& GetInstanceVK();
		vk::Device& GetDeviceVK();
		vk::PhysicalDevice& GetPrimaryAdapterVK();

		vk::Queue& GetGraphicsQueueVK();
		vk::Queue& GetPresentQueueVK();
		vk::Queue& GetComputeQueueVK();
		uint32_t GetGraphicsQueueFamilyIndexVK() const;
		uint32_t GetPresentQueueFamilyIndexVK() const;

		uint32_t GetMemoryTypeIndexVK(uint32_t filter, vk::MemoryPropertyFlags flags);

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
		vk::Queue mComputeQueue;

		uint32_t mGraphicsQueueFamilyIndex;
		uint32_t mPresentQueueFamilyIndex;
		uint32_t mComputeQueueFamilyIndex;

		MSAALevel mSupportedMaxMSAA = MSAALevel::x8;
	};
}
#endif