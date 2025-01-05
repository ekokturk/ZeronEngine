// Copyright (C) Eser Kokturk. All Rights Reserved.


#if ZE_GRAPHICS_D3D

#	include <Graphics/API/D3D/11/GraphicsD3D11.h>

#	include <Graphics/API/D3D/11/BufferD3D11.h>
#	include <Graphics/API/D3D/11/CommandBufferD3D11.h>
#	include <Graphics/API/D3D/11/D3D11Helpers.h>
#	include <Graphics/API/D3D/11/FrameBufferD3D11.h>
#	include <Graphics/API/D3D/11/GraphicsContextD3D11.h>
#	include <Graphics/API/D3D/11/PipelineBindingD3D11.h>
#	include <Graphics/API/D3D/11/PipelineD3D11.h>
#	include <Graphics/API/D3D/11/RenderPassD3D11.h>
#	include <Graphics/API/D3D/11/SamplerD3D11.h>
#	include <Graphics/API/D3D/11/ShaderD3D11.h>
#	include <Graphics/API/D3D/11/ShaderProgramD3D11.h>
#	include <Graphics/API/D3D/11/SwapChainD3D11.h>
#	include <Graphics/API/D3D/11/TextureD3D11.h>
#	include <Graphics/API/D3D/DebugInfoD3D.h>
#	include <Graphics/Primitives.h>
#	include <Graphics/ShaderProgramConfig.h>

#	include <d3d11.h>

namespace Zeron::Gfx
{
	GraphicsD3D11::GraphicsD3D11()
		: mMaxSupportedSampling(MSAALevel::Disabled)
	{}

	GraphicsD3D11::~GraphicsD3D11() = default;

	bool GraphicsD3D11::Init()
	{
		const auto& adapters = GetAdapters();
		if (adapters.empty()) {
			ZE_FAIL("No graphics adapter found for Direct3D!");
			return false;
		}

		IDXGIAdapter* primaryAdapter = adapters[0].GetAdapterD3D();
		ZE_D3D_ASSERT_RESULT(primaryAdapter->GetParent(__uuidof(IDXGIFactory), &mFactory), false);
		ZE_LOG("Direct3D11 GFX: Using '{}' device", adapters[0].GetName());

		UINT creationFlags = 0;
#	if ZE_DEBUG
		creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#	endif
		ZE_D3D_ASSERT_RESULT(
			D3D11CreateDevice(primaryAdapter, D3D_DRIVER_TYPE_UNKNOWN, nullptr, creationFlags, nullptr, 0, D3D11_SDK_VERSION, &mDevice, nullptr, &mDeviceContext), false
		);

		ZE_LOGI("Graphics API: Direct3D 11");

		mImmediateContext = std::make_shared<GraphicsContextD3D11>(*this);

		mMaxSupportedSampling = _getMaxMultiSampleLevel();

		return true;
	}

	GraphicsType GraphicsD3D11::GetGraphicsType() const
	{
		return GraphicsType::Direct3D11;
	}

	MSAALevel GraphicsD3D11::GetMultiSamplingLevel() const
	{
		return mMaxSupportedSampling;
	}

	std::unique_ptr<GraphicsContext> GraphicsD3D11::CreateGraphicsContext()
	{
		return std::make_unique<GraphicsContextD3D11>(*this);
	}

	std::unique_ptr<CommandBuffer> GraphicsD3D11::CreateCommandBuffer(uint32_t count, bool isCompute)
	{
		return std::make_unique<CommandBufferD3D11>(*this);
	}

	std::unique_ptr<Pipeline> GraphicsD3D11::CreatePipelineGraphics(ShaderProgram* shader, RenderPass* renderPass, PipelineConfig config)
	{
		return std::make_unique<PipelineD3D11>(*this, static_cast<ShaderProgramD3D11*>(shader), renderPass, std::move(config));
	}

	std::unique_ptr<Pipeline> GraphicsD3D11::CreatePipelineCompute(ShaderProgram& shader)
	{
		return std::make_unique<PipelineD3D11>(*this, static_cast<ShaderProgramD3D11&>(shader));
	}

	std::unique_ptr<PipelineBinding> GraphicsD3D11::CreatePipelineBinding(Pipeline& pipeline, const std::vector<BindingHandle>& bindingList)
	{
		return std::make_unique<PipelineBindingD3D11>(*this, static_cast<PipelineD3D11&>(pipeline), bindingList);
	}
	std::unique_ptr<RenderPass> GraphicsD3D11::CreateRenderPass(
		std::vector<RenderPassAttachment> colorAttachments, std::optional<RenderPassAttachment> depthAttachment, MSAALevel sampling
	)
	{
		return std::make_unique<RenderPassD3D11>(*this, std::move(colorAttachments), depthAttachment, sampling);
	}

	std::unique_ptr<FrameBuffer> GraphicsD3D11::CreateFrameBuffer(
		RenderPass& renderPass, const Vec2i& extent, const std::span<Texture*>& colorTextures, Texture* depthTexture, const std::span<Texture*>& resolveTextures
	)
	{
		return std::make_unique<FrameBufferD3D11>(*this, static_cast<RenderPassD3D11&>(renderPass), extent, colorTextures, depthTexture, resolveTextures);
	}

	std::unique_ptr<Buffer> GraphicsD3D11::CreateBuffer(BufferType type, uint32_t size, uint32_t stride, const void* data, BufferUsageType usage)
	{
		return std::make_unique<BufferD3D11>(*this, type, size, stride, data, usage);
	}

	std::unique_ptr<ShaderProgram> GraphicsD3D11::CreateShaderProgram(const ShaderProgramConfig& config, std::unordered_map<ShaderType, ByteBuffer> shaderData)
	{
		return std::make_unique<ShaderProgramD3D11>(*this, config, std::move(shaderData));
	}

	std::string GraphicsD3D11::GetCompiledShaderName(const std::string& shaderName, ShaderType type) const
	{
		switch (type) {
			case ShaderType::Vertex: return { shaderName + ".vert.cso" };
			case ShaderType::Fragment: return { shaderName + ".frag.cso" };
			case ShaderType::Compute: return { shaderName + ".comp.cso" };
			default: ZE_FAIL("Vulkan compiled shader name is not implemented!");
		}
		return shaderName;
	}

	std::unique_ptr<Texture> GraphicsD3D11::CreateTexture(const Vec2i& size, TextureFormat format, const void* data, TextureType type, MSAALevel sampling)
	{
		return std::make_unique<TextureD3D11>(*this, size, type, format, data, sampling, true);
	}

	std::unique_ptr<Sampler> GraphicsD3D11::CreateSampler(SamplerAddressMode addressMode, bool hasAnisotropicFilter)
	{
		return std::make_unique<SamplerD3D11>(*this, addressMode, hasAnisotropicFilter);
	}

	const std::vector<GraphicsAdapterD3D11>& GraphicsD3D11::GetAdapters()
	{
		if (mGraphicsAdapters.empty()) {
			Gfx::ComPtr<IDXGIFactory> dxgiFactory;
			ZE_D3D_ASSERT_RESULT(CreateDXGIFactory(__uuidof(IDXGIFactory), reinterpret_cast<void**>(dxgiFactory.GetAddressOf())), mGraphicsAdapters);
			Gfx::ComPtr<IDXGIAdapter> dxgiAdapter;
			UINT index = 0;
			while (SUCCEEDED(dxgiFactory->EnumAdapters(index, &dxgiAdapter))) {
				mGraphicsAdapters.emplace_back(GraphicsAdapterD3D11(dxgiAdapter));
				index++;
			}
		}
		return mGraphicsAdapters;
	}

	IDXGIFactory* GraphicsD3D11::GetFactoryD3D() const
	{
		return mFactory.Get();
	}

	IDXGIAdapter* GraphicsD3D11::GetPrimaryAdapterD3D()
	{
		auto& adapterList = GetAdapters();
		ZE_ASSERT(!adapterList.empty(), "No graphics adapters found!");
		return adapterList[0].GetAdapterD3D();
	}

	ID3D11Device* GraphicsD3D11::GetDeviceD3D() const
	{
		return mDevice.Get();
	}

	ID3D11DeviceContext* GraphicsD3D11::GetDeviceContextD3D() const
	{
		return mDeviceContext.Get();
	}

	MSAALevel GraphicsD3D11::_getMaxMultiSampleLevel() const
	{
		UINT qualityLevels = 0;
		for (UINT i = 8 /* D3D11_MAX_MULTISAMPLE_SAMPLE_COUNT */; i > 1; i = i / 2) {
			ZE_D3D_ASSERT_RESULT(mDevice->CheckMultisampleQualityLevels(D3D11Helpers::GetTextureFormat(TextureFormat::BGRA_8U), 8, &qualityLevels), MSAALevel::Disabled);
			switch (i) {
				case 8: return MSAALevel::x8;
				case 4: return MSAALevel::x4;
				case 2: return MSAALevel::x2;
			}
		}
		return MSAALevel::Disabled;
	}
}
#endif