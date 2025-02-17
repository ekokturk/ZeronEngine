// Copyright (C) Eser Kokturk. All Rights Reserved.

#if ZE_GRAPHICS_VULKAN

#	include <Graphics/API/Vulkan/GraphicsVulkan.h>

#	include <Graphics/API/Vulkan/BufferVulkan.h>
#	include <Graphics/API/Vulkan/CommandBufferVulkan.h>
#	include <Graphics/API/Vulkan/FrameBufferVulkan.h>
#	include <Graphics/API/Vulkan/GraphicsContextVulkan.h>
#	include <Graphics/API/Vulkan/PipelineBindingVulkan.h>
#	include <Graphics/API/Vulkan/PipelineVulkan.h>
#	include <Graphics/API/Vulkan/RenderPassVulkan.h>
#	include <Graphics/API/Vulkan/SamplerVulkan.h>
#	include <Graphics/API/Vulkan/ShaderProgramVulkan.h>
#	include <Graphics/API/Vulkan/ShaderVulkan.h>
#	include <Graphics/API/Vulkan/SurfaceVulkan.h>
#	include <Graphics/API/Vulkan/SwapChainVulkan.h>
#	include <Graphics/API/Vulkan/TextureVulkan.h>
#	include <Graphics/API/Vulkan/VulkanDebug.h>
#	include <Graphics/API/Vulkan/VulkanInstance.h>
#	include <Graphics/VertexLayout.h>

VULKAN_HPP_DEFAULT_DISPATCH_LOADER_DYNAMIC_STORAGE

namespace Zeron::Gfx
{
	GraphicsVulkan::GraphicsVulkan()
		: mDevice(nullptr)
		, mGraphicsQueueFamilyIndex(UINT_MAX)
		, mPresentQueueFamilyIndex(UINT_MAX)
	{
		mAppInfo = vk::ApplicationInfo("AppName", 1, "Zeron Engine", 1, VK_API_VERSION_1_1);

		mExtensions.emplace_back(VK_KHR_SURFACE_EXTENSION_NAME);
#	if ZE_PLATFORM_WIN32
		mExtensions.emplace_back("VK_KHR_win32_surface");
#	elif ZE_PLATFORM_LINUX
		mExtensions.emplace_back("VK_KHR_xlib_surface");
#	elif ZE_PLATFORM_ANDROID
		mExtensions.emplace_back("VK_KHR_android_surface");
#	endif
#	if ZE_DEBUG
#		if ZE_PLATFORM_ANDROID
		mExtensions.emplace_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
#		else
		mExtensions.emplace_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
#		endif
#	endif
	}

	GraphicsVulkan::~GraphicsVulkan()
	{
		if (mDevice) {
			mDevice.waitIdle();
			mDevice.destroy();
		}
		VulkanInstance::DestroyRef();
	}

	bool GraphicsVulkan::Init()
	{
		VulkanInstance::LoadProc();

		if (!_initInstance()) {
			ZE_LOGE("GraphicsVulkan: Couldn't create instance!");
			return false;
		}
		if (!_initPrimaryAdapter()) {
			ZE_LOGE("GraphicsVulkan: Couldn't create physical device!");
			return false;
		}

		return true;
	}

	GraphicsType GraphicsVulkan::GetGraphicsType() const
	{
		return GraphicsType::Vulkan;
	}

	MSAALevel GraphicsVulkan::GetMultiSamplingLevel() const
	{
		return mMaxSupportedSampling;
	}

	std::unique_ptr<GraphicsContext> GraphicsVulkan::CreateGraphicsContext()
	{
		return std::make_unique<GraphicsContextVulkan>(*this);
	}

	std::unique_ptr<CommandBuffer> GraphicsVulkan::CreateCommandBuffer(uint32_t count, bool isCompute)
	{
		return std::make_unique<CommandBufferVulkan>(*this, count, isCompute);
	}

	std::unique_ptr<Pipeline> GraphicsVulkan::CreatePipelineGraphics(ShaderProgram* shader, RenderPass* renderPass, PipelineConfig config)
	{
		ZE_ASSERT(shader, "GraphicsVulkan: Vulkan pipeline requires a valid shader program!");
		ZE_ASSERT(renderPass, "GraphicsVulkan: Vulkan pipeline requires a valid render pass!");

		config.mSamplingLevel = static_cast<int>(mMaxSupportedSampling) < static_cast<int>(config.mSamplingLevel) ? mMaxSupportedSampling : config.mSamplingLevel;

		return std::make_unique<PipelineVulkan>(*this, static_cast<ShaderProgramVulkan*>(shader), static_cast<RenderPassVulkan*>(renderPass), std::move(config));
	}

	std::unique_ptr<Pipeline> GraphicsVulkan::CreatePipelineCompute(ShaderProgram& shader)
	{
		return std::make_unique<PipelineVulkan>(*this, static_cast<ShaderProgramVulkan*>(&shader));
	}

	std::unique_ptr<PipelineBinding> GraphicsVulkan::CreatePipelineBinding(Pipeline& pipeline, const std::vector<BindingHandle>& bindingList)
	{
		return std::make_unique<PipelineBindingVulkan>(*this, static_cast<PipelineVulkan&>(pipeline), bindingList);
	}
	std::unique_ptr<RenderPass> GraphicsVulkan::CreateRenderPass(
		std::vector<RenderPassAttachment> colorAttachments, std::optional<RenderPassAttachment> depthAttachment, MSAALevel sampling
	)
	{
		return std::make_unique<RenderPassVulkan>(*this, std::move(colorAttachments), depthAttachment, sampling);
	}

	std::unique_ptr<FrameBuffer> GraphicsVulkan::CreateFrameBuffer(
		RenderPass& renderPass, const Vec2i& extent, const std::span<Texture*>& colorAttachments, Texture* depthAttachment, const std::span<Texture*>& resolveAttachments
	)
	{
		return std::make_unique<FrameBufferVulkan>(*this, static_cast<RenderPassVulkan&>(renderPass), extent, colorAttachments, depthAttachment, resolveAttachments);
	}

	std::unique_ptr<Sampler> GraphicsVulkan::CreateSampler(SamplerAddressMode addressMode, bool hasAnisotropicFilter)
	{
		return std::make_unique<SamplerVulkan>(*this, addressMode, hasAnisotropicFilter);
	}

	std::unique_ptr<Buffer> GraphicsVulkan::CreateBuffer(BufferType type, uint32_t count, uint32_t stride, const void* data, BufferUsageType usage)
	{
		if (usage == BufferUsageType::Dynamic || usage == BufferUsageType::Staging) {
			return std::make_unique<BufferVulkan>(*this, type, count, stride, data, usage);
		}
		auto deviceLocalBuffer = std::make_unique<BufferVulkan>(*this, type, count, stride, nullptr, usage);
		if (data) {
			auto stagingBuffer = std::make_unique<BufferVulkan>(*this, BufferType::Undefined, count, stride, data, BufferUsageType::Staging);
			SubmitSingleUseCommandBufferVK([&](CommandBufferVulkan& cmd) {
				cmd.CopyBuffer(*stagingBuffer, *deviceLocalBuffer);
			});
		}
		return deviceLocalBuffer;
	}

	std::unique_ptr<ShaderProgram> GraphicsVulkan::CreateShaderProgram(const ShaderProgramConfig& config, std::unordered_map<ShaderType, ByteBuffer> shaderData)
	{
		return std::make_unique<ShaderProgramVulkan>(*this, config, std::move(shaderData));
	}

	std::string GraphicsVulkan::GetCompiledShaderName(const std::string& shaderName, ShaderType type) const
	{
		switch (type) {
			case ShaderType::Vertex: return { shaderName + ".vert.spv" };
			case ShaderType::Fragment: return { shaderName + ".frag.spv" };
			case ShaderType::Compute: return { shaderName + ".comp.spv" };
			default: ZE_FAIL("GraphicsVulkan: Compiled shader name is not implemented!");
		}
		return shaderName;
	}

	std::unique_ptr<Texture> GraphicsVulkan::CreateTexture(const Vec2i& size, TextureFormat format, const void* data, TextureType type, MSAALevel sampling)
	{
		vk::ImageUsageFlags usageFlags = vk::ImageUsageFlagBits::eSampled;
		if (type == TextureType::Depth) {
			usageFlags |= vk::ImageUsageFlagBits::eDepthStencilAttachment;
		}
		if (data) {
			usageFlags |= vk::ImageUsageFlagBits::eTransferDst;
			usageFlags |= vk::ImageUsageFlagBits::eTransferSrc;
		}

		return CreateTextureVK(
			size, type, format, data, sampling, type == TextureType::Depth ? TextureLayout::DepthStencilAttachment : TextureLayout::ShaderReadOnly, usageFlags
		);
	}

	std::unique_ptr<TextureVulkan> GraphicsVulkan::CreateTextureVK(
		const Vec2i& size, TextureType type, TextureFormat format, const void* data, MSAALevel sampling, TextureLayout layout, vk::ImageUsageFlags usage
	)
	{
		auto texture = std::make_unique<TextureVulkan>(*this, size, type, format, sampling, usage);
		const std::unique_ptr<BufferVulkan> stagingBuffer = data ?
			std::make_unique<BufferVulkan>(*this, BufferType::Undefined, texture->GetCount(), texture->GetStride(), data, BufferUsageType::Staging) :
			nullptr;
		const TextureLayout oldLayoutModified = data ? TextureLayout::TransferDst : TextureLayout::Undefined;
		SubmitSingleUseCommandBufferVK([&](CommandBufferVulkan& cmd) {
			if (data) {
				cmd.ApplyImageTransitionLayoutVK(*texture, TextureLayout::Undefined, TextureLayout::TransferDst);
				cmd.CopyBufferToTextureVK(*stagingBuffer, *texture);
			}
			cmd.ApplyImageTransitionLayoutVK(*texture, oldLayoutModified, layout);
		});
		return texture;
	}

	vk::Instance& GraphicsVulkan::GetInstanceVK()
	{
		ZE_ASSERT(mInstance, "GraphicsVulkan: Vulkan instance is not available!");
		return mInstance;
	}

	vk::Device& GraphicsVulkan::GetDeviceVK()
	{
		ZE_ASSERT(mDevice, "GraphicsVulkan: Vulkan device is not available!");
		return mDevice;
	}

	vk::PhysicalDevice& GraphicsVulkan::GetPrimaryAdapterVK()
	{
		ZE_ASSERT(mAdapter, "GraphicsVulkan: Couldn't find Vulkan adapter!");
		return mAdapter->GetPhysicalDeviceVK();
	}

	vk::Queue& GraphicsVulkan::GetGraphicsQueueVK()
	{
		return mGraphicsQueue;
	}

	vk::Queue& GraphicsVulkan::GetPresentQueueVK()
	{
		return mPresentQueue;
	}

	uint32_t GraphicsVulkan::GetGraphicsQueueFamilyIndexVK() const
	{
		return mGraphicsQueueFamilyIndex;
	}

	uint32_t GraphicsVulkan::GetPresentQueueFamilyIndexVK() const
	{
		return mPresentQueueFamilyIndex;
	}

	bool GraphicsVulkan::_verifyExtensions() const
	{
		// Get all supported extensions
		auto supportedExtensions = vk::enumerateInstanceExtensionProperties(nullptr);
		ZE_ASSERT(!supportedExtensions.empty(), "GraphicsVulkan: Expected to find supported extensions");

		for (const auto& extensionName : mExtensions) {
			auto result = std::find_if(supportedExtensions.begin(), supportedExtensions.end(), [&extensionName](const vk::ExtensionProperties& props) {
				return extensionName == props.extensionName;
			});
			if (result == supportedExtensions.end()) {
				ZE_LOGE("GraphicsVulkan: Couldn't find extension '{}'!", extensionName);
				return false;
			}
		}
		return true;
	}

	void GraphicsVulkan::_initSupportedValidationLayers()
	{
#	if ZE_DEBUG
		const char* expectedLayers[] = { "VK_LAYER_KHRONOS_validation" };
		// Get all supported validation layers
		auto supportedLayers = vk::enumerateInstanceLayerProperties();
		ZE_ASSERT(!supportedLayers.empty(), "GraphicsVulkan: Expected to find validation layers");

		for (auto& expectedLayer : expectedLayers) {
			auto result = std::find_if(supportedLayers.begin(), supportedLayers.end(), [&expectedLayer](const vk::LayerProperties& prop) {
				return std::strcmp(expectedLayer, prop.layerName) == 0;
			});
			if (result != supportedLayers.end()) {
				mValidationLayers.emplace_back(expectedLayer);
			}
			else {
				ZE_LOGE("GraphicsVulkan: Couldn't find validation layer '{}'!", expectedLayer);
			}
		}
#	endif
	}

	std::vector<GraphicsAdapterVulkan> GraphicsVulkan::_getGraphicsAdapters() const
	{
		std::vector<GraphicsAdapterVulkan> adapters;
		for (const auto& adapter : mInstance.enumeratePhysicalDevices()) {
			adapters.emplace_back(adapter);
		}
		return adapters;
	}

	void GraphicsVulkan::SubmitSingleUseCommandBufferVK(std::function<void(CommandBufferVulkan&)> commands)
	{
		auto cmd = CreateCommandBuffer(1, false);
		auto& vkCmd = static_cast<CommandBufferVulkan&>(*cmd);
		vkCmd.Begin();
		commands(vkCmd);
		vkCmd.End();
		vk::UniqueFence fence = mDevice.createFenceUnique({ vk::FenceCreateFlags() });
		const vk::SubmitInfo submitInfo(0, nullptr, nullptr, 1, &vkCmd.GetCommandBufferVK(), 0, nullptr);
		ZE_VK_ASSERT(GetGraphicsQueueVK().submit(1, &submitInfo, fence.get()), "Vulkan command buffer submission failed!");
		ZE_VK_ASSERT(mDevice.waitForFences(fence.get(), VK_TRUE, ZE_VK_TIMEOUT), "Failed to wait Vulkan fence!");
		ZE_VK_ASSERT(mDevice.resetFences(1, &fence.get()), "Failed to reset Vulkan fence!");
	}

	bool GraphicsVulkan::_initInstance()
	{
		uint32_t instanceVersion = 0;
		if (vk::enumerateInstanceVersion(&instanceVersion) != vk::Result::eSuccess) {
			ZE_LOGE("GraphicsVulkan: Unable to retrieve version information!");
			return false;
		}

		const uint32_t versionMajor = VK_VERSION_MAJOR(instanceVersion);
		const uint32_t versionMinor = VK_VERSION_MINOR(instanceVersion);
		const uint32_t versionPatch = VK_VERSION_PATCH(instanceVersion);
		const bool hasValidVersion = versionMajor > 1 || versionMajor == 1 && versionMinor >= 1;
		if (!hasValidVersion) {
			ZE_LOGE("GraphicsVulkan: Minimum Vulkan version 1.1 required (Current supported version {}.{})", versionMajor, versionMinor);
			return false;
		}

		ZE_LOG("Vulkan GFX: Found supported library version {}.{}.{}", versionMajor, versionMinor, versionPatch);

		_initSupportedValidationLayers();
		if (!_verifyExtensions()) {
			ZE_FAIL("Couldn't find requested Vulkan extension among supported extensions!");
			return false;
		}
		std::vector<const char*> validationLayers(mValidationLayers.size());
		std::transform(mValidationLayers.begin(), mValidationLayers.end(), validationLayers.begin(), [](const std::string& s) {
			return s.c_str();
		});
		std::vector<const char*> extensions(mExtensions.size());
		std::transform(mExtensions.begin(), mExtensions.end(), extensions.begin(), [](const std::string& s) {
			return s.c_str();
		});

		const vk::InstanceCreateInfo instanceCreateInfo{ vk::InstanceCreateFlags(),
														 &mAppInfo,
														 static_cast<uint32_t>(validationLayers.size()),
														 validationLayers.empty() ? nullptr : validationLayers.data(),
														 static_cast<uint32_t>(extensions.size()),
														 extensions.empty() ? nullptr : extensions.data() };

		mInstance = VulkanInstance::CreateRef(instanceCreateInfo);
		return mInstance;
	}

	bool GraphicsVulkan::_initPrimaryAdapter()
	{
		std::vector<GraphicsAdapterVulkan> adapters = _getGraphicsAdapters();
		if (adapters.empty()) {
			ZE_FAIL("No Physical device found for Vulkan!");
			return false;
		}

		// Try to find a discrete GPU
		const auto itr = std::find_if(adapters.begin(), adapters.end(), [](const GraphicsAdapterVulkan& adapter) {
			return adapter.IsDiscreteAdapter();
		});
		if (itr != adapters.end()) {
			mAdapter = std::make_unique<GraphicsAdapterVulkan>(*itr);
		}
		else {
			mAdapter = std::make_unique<GraphicsAdapterVulkan>(adapters[0]);
		}

		if (!mAdapter->HasSwapChainSupport()) {
			ZE_FAIL("Physical device does not have swap chain support!");
			return false;
		}

		mMaxSupportedSampling = _getMaxMultiSampleLevel();

		ZE_LOG("Vulkan GFX: Using '{}' device", mAdapter->GetName());
		return mAdapter->GetPhysicalDeviceVK();
	}

	bool GraphicsVulkan::_initDevice(vk::SurfaceKHR surface)
	{
		ZE_ASSERT(!mDevice, "GraphicsVulkan: Vulkan logical device already exists");
		const vk::PhysicalDevice& adapter = mAdapter->GetPhysicalDeviceVK();
		std::vector<vk::QueueFamilyProperties> queueFamilies = adapter.getQueueFamilyProperties();
		// Find graphics queue family that can present
		for (uint32_t i = 0; i < queueFamilies.size(); ++i) {
			vk::QueueFamilyProperties& props = queueFamilies[i];
			// We don't use dedicated compute queue
			const auto graphicsQueueFlags = vk::QueueFlagBits::eGraphics | vk::QueueFlagBits::eCompute;
			if (props.queueCount > 0 && props.queueFlags & graphicsQueueFlags) {
				if (mGraphicsQueueFamilyIndex == UINT_MAX) {
					mGraphicsQueueFamilyIndex = i;
				}
				if (adapter.getSurfaceSupportKHR(i, surface)) {
					mGraphicsQueueFamilyIndex = i;
					mPresentQueueFamilyIndex = i;
					break;
				}
			}
		}
		if (mGraphicsQueueFamilyIndex == UINT_MAX) {
			ZE_FAIL("GraphicsVulkan: Couldn't find Vulkan graphics queue family!");
			return false;
		}
		// Try to find present queue family again if it's not found as a graphics queue
		if (mPresentQueueFamilyIndex == UINT_MAX) {
			for (uint32_t i = 0; i < queueFamilies.size(); ++i) {
				if (adapter.getSurfaceSupportKHR(i, surface)) {
					mPresentQueueFamilyIndex = i;
					break;
				}
			}
		}
		if (mPresentQueueFamilyIndex == UINT_MAX) {
			ZE_FAIL("GraphicsVulkan: Couldn't find required Vulkan presentation queue family!");
			return false;
		}

		const float queuePriority = 1.f;
		std::vector<vk::DeviceQueueCreateInfo> queueCreateInfos{ { vk::DeviceQueueCreateFlags(), mGraphicsQueueFamilyIndex, 1, &queuePriority } };
		if (mPresentQueueFamilyIndex != mGraphicsQueueFamilyIndex) {
			queueCreateInfos.emplace_back(vk::DeviceQueueCreateFlags(), mPresentQueueFamilyIndex, 1, &queuePriority);
		}


		const vk::PhysicalDeviceFeatures supportedFeatures = adapter.getFeatures();
		vk::PhysicalDeviceFeatures enabledFeatures{};
		// Shader based multi-sampling
		if (supportedFeatures.sampleRateShading) {
			enabledFeatures.sampleRateShading = true;
		}
		// Anisotropic Filtering
		if (supportedFeatures.samplerAnisotropy) {
			enabledFeatures.samplerAnisotropy = true;
		}
		// Non Solid fill
		if (supportedFeatures.fillModeNonSolid) {
			enabledFeatures.fillModeNonSolid = true;
		}

		std::vector<const char*> extensionNames{ VK_KHR_SWAPCHAIN_EXTENSION_NAME };
		const vk::DeviceCreateInfo deviceCreateInfo(
			vk::DeviceCreateFlags(),
			static_cast<uint32_t>(queueCreateInfos.size()),
			queueCreateInfos.data(),
			0,
			nullptr,
			static_cast<uint32_t>(extensionNames.size()),
			extensionNames.data(),
			&enabledFeatures
		);

		mDevice = adapter.createDevice(deviceCreateInfo);
		mGraphicsQueue = mDevice.getQueue(mGraphicsQueueFamilyIndex, 0);
		mPresentQueue = mDevice.getQueue(mPresentQueueFamilyIndex, 0);

#	if ZE_DEBUG && !ZE_PLATFORM_ANDROID
		mObjectDebugInterface = std::make_unique<VulkanObjectDebugInterface>(mDevice);
#	endif
		return mDevice;
	}

	std::unique_ptr<SwapChainVulkan> GraphicsVulkan::CreateSwapChainVK(SystemHandle systemHandle, const Vec2i& size)
	{
		vk::UniqueSurfaceKHR surface = SurfaceVulkan::CreateSurfaceVK(mInstance, systemHandle);
		if (!mDevice) {
			_initDevice(*surface);
		}
		return std::make_unique<SwapChainVulkan>(*this, systemHandle, size, std::move(surface));
	}

	vk::UniqueCommandPool GraphicsVulkan::CreateCommandPoolVK() const
	{
		return mDevice.createCommandPoolUnique(vk::CommandPoolCreateInfo{ vk::CommandPoolCreateFlagBits::eResetCommandBuffer, mGraphicsQueueFamilyIndex });
	}

	vk::UniqueDescriptorPool GraphicsVulkan::CreateDescriptorPoolVK(const std::vector<vk::DescriptorPoolSize>& poolSizeList) const
	{
		ZE_ASSERT(!poolSizeList.empty(), "Expected atleast one Vulkan descriptor pool size!");
		const auto maxPool = std::max_element(poolSizeList.begin(), poolSizeList.end(), [](const vk::DescriptorPoolSize& lhs, const vk::DescriptorPoolSize& rhs) {
			return lhs.descriptorCount < rhs.descriptorCount;
		});
		const vk::DescriptorPoolCreateInfo info(
			vk::DescriptorPoolCreateFlagBits::eFreeDescriptorSet, maxPool->descriptorCount, static_cast<uint32_t>(poolSizeList.size()), poolSizeList.data()
		);
		return mDevice.createDescriptorPoolUnique(info);
	}

	uint32_t GraphicsVulkan::GetMemoryTypeIndexVK(uint32_t filter, vk::MemoryPropertyFlags flags)
	{
		const vk::PhysicalDeviceMemoryProperties memoryProperties = GetPrimaryAdapterVK().getMemoryProperties();
		for (uint32_t index = 0; index < memoryProperties.memoryTypeCount; index++) {
			if ((filter & (1 << index)) && (memoryProperties.memoryTypes[index].propertyFlags & flags) == flags) {
				return index;
			}
		}
		ZE_FAIL("Vulkan memory type couldn't be found!");
	}

#	if ZE_DEBUG
	VulkanObjectDebugInterface* GraphicsVulkan::getObjectDebugInterface() const
	{
		return mObjectDebugInterface.get();
	}
#	endif

	MSAALevel GraphicsVulkan::_getMaxMultiSampleLevel()
	{
		const vk::PhysicalDeviceProperties props = GetPrimaryAdapterVK().getProperties();
		const vk::SampleCountFlags sampleCountMask{ props.limits.framebufferColorSampleCounts };
		if (vk::SampleCountFlagBits::e8 & sampleCountMask) {
			return MSAALevel::x8;
		}
		if (vk::SampleCountFlagBits::e4 & sampleCountMask) {
			return MSAALevel::x4;
		}
		if (vk::SampleCountFlagBits::e2 & sampleCountMask) {
			return MSAALevel::x2;
		}
		if (vk::SampleCountFlagBits::e1 & sampleCountMask) {
			return MSAALevel::Disabled;
		}
		ZE_FAIL("Vulkan multi-sampling anti aliasing level is not supported!");
		return MSAALevel::Disabled;
	}
}
#endif