// Copyright (C) Eser Kokturk. All Rights Reserved.

#if ZE_GRAPHICS_D3D
#include <Graphics/API/D3D/11/GraphicsD3D11.h>

#include <Graphics/Primitives.h>
#include <Graphics/API/D3D/DebugInfoD3D.h>
#include <Graphics/API/D3D/11/BufferD3D11.h>
#include <Graphics/API/D3D/11/CommandBufferD3D11.h>
#include <Graphics/API/D3D/11/D3D11Helpers.h>
#include <Graphics/API/D3D/11/GraphicsContextD3D11.h>
#include <Graphics/API/D3D/11/PipelineBindingD3D11.h>
#include <Graphics/API/D3D/11/PipelineD3D11.h>
#include <Graphics/API/D3D/11/SamplerD3D11.h>
#include <Graphics/API/D3D/11/ShaderD3D11.h>
#include <Graphics/API/D3D/11/SwapChainD3D11.h>
#include <Graphics/API/D3D/11/TextureD3D11.h>

#include <d3d11.h>

namespace Zeron
{
	GraphicsD3D11::GraphicsD3D11() = default;

	GraphicsD3D11::~GraphicsD3D11() = default;

	bool GraphicsD3D11::Init()
	{
		const auto& adapters = GetAdapters();
		if(adapters.empty()) {
			ZE_FAIL("No graphics adapter found for Direct3D!");
			return false;
		}

		IDXGIAdapter* primaryAdapter = adapters[0].GetAdapterD3D();
		D3D_ASSERT_RESULT(primaryAdapter->GetParent(__uuidof(IDXGIFactory), &mFactory), false);
		ZE_LOG("Using '{}' device for Direct3D 11", adapters[0].GetName());

		UINT creationFlags = 0;
	#if ZE_DEBUG
		creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
	#endif
		D3D_ASSERT_RESULT(D3D11CreateDevice(
			primaryAdapter,
			D3D_DRIVER_TYPE_UNKNOWN,
			nullptr,
			creationFlags,
			nullptr,
			0,
			D3D11_SDK_VERSION,
			&mDevice,
			nullptr,
			&mDeviceContext
		), false);

		ZE_LOGI("Graphics API: Direct3D 11");

		mImmediateContext = std::make_shared<GraphicsContextD3D11>(*this);

		return true;
	}

	GraphicsType GraphicsD3D11::GetGraphicsType() const
	{
		return GraphicsType::Direct3D11;
	}

	MSAALevel GraphicsD3D11::GetMultiSamplingLevel() const
	{
		// TODO: Make use of D3D11_MAX_MULTISAMPLE_SAMPLE_COUNT
		return MSAALevel::x8;
	}

	std::unique_ptr<GraphicsContext> GraphicsD3D11::CreateGraphicsContext()
	{
		return std::make_unique<GraphicsContextD3D11>(*this);
	}

	std::unique_ptr<CommandBuffer> GraphicsD3D11::CreateCommandBuffer(uint32_t count, bool isCompute)
	{
		return std::make_unique<CommandBufferD3D11>(*this);
	}

	std::unique_ptr<Pipeline> GraphicsD3D11::CreatePipeline(ShaderProgram* shader)
	{
		ZE_FAIL("Not implemented yet!");
		return nullptr;
	}

	std::unique_ptr<Pipeline> GraphicsD3D11::CreatePipeline(ShaderProgram* shader, RenderPass* renderPass,
	                                                        MSAALevel samplingLevel, PrimitiveTopology topology, bool isSolidFill, FaceCullMode cullMode)
	{
		return std::make_unique<PipelineD3D11>(*this, static_cast<ShaderProgramD3D11*>(shader),
			renderPass, samplingLevel, topology, isSolidFill, cullMode);
	}

	std::unique_ptr<PipelineBinding> GraphicsD3D11::CreatePipelineBinding(Pipeline& pipeline, const std::vector<BindingHandle>& bindingList)
	{
		return std::make_unique<PipelineBindingD3D11>(*this, static_cast<PipelineD3D11&>(pipeline), bindingList);
	}

	std::unique_ptr<Buffer> GraphicsD3D11::CreateBuffer(BufferType type, uint32_t size, uint32_t stride, const void* data, BufferUsageType usage)
	{
		return std::make_unique<BufferD3D11>(*this, type, size, stride, data, usage);
	}

	std::unique_ptr<ShaderProgram> GraphicsD3D11::CreateShaderProgram(const std::string& shaderName,
		const std::string& shaderDirectory, const VertexLayout& vertexLayout, const ResourceLayout& resourceLayout)
	{
		return std::make_unique<ShaderProgramD3D11>(*this, shaderName, shaderDirectory, vertexLayout, resourceLayout);
	}

	std::unique_ptr<ShaderProgram> GraphicsD3D11::CreateShaderProgram(const std::string& shaderName,
		const std::shared_ptr<Shader>& vertexShader, const std::shared_ptr<Shader>& fragmentShader,
		const VertexLayout& vertexLayout, const ResourceLayout& resourceLayout)
	{
		return std::make_unique<ShaderProgramD3D11>(*this, shaderName, vertexShader, fragmentShader, vertexLayout, resourceLayout);
	}

	std::unique_ptr<Texture> GraphicsD3D11::CreateTexture(TextureType type, const Color& data)
	{
		return std::make_unique<TextureD3D11>(*this, type, &data, 1, 1);
	}

	std::unique_ptr<Texture> GraphicsD3D11::CreateTexture(TextureType type, const Color* data, uint32_t width, uint32_t height)
	{
		return std::make_unique<TextureD3D11>(*this, type, data, width, height);
	}

	std::unique_ptr<Sampler> GraphicsD3D11::CreateSampler(SamplerAddressMode addressMode, bool hasAnisotropicFilter)
	{
		return std::make_unique<SamplerD3D11>(*this, addressMode, hasAnisotropicFilter);
	}

	const std::vector<GraphicsAdapterD3D11>& GraphicsD3D11::GetAdapters()
	{
		if (mGraphicsAdapters.empty()) {
			ZE::ComPtr<IDXGIFactory> dxgiFactory;
			D3D_ASSERT_RESULT(CreateDXGIFactory(__uuidof(IDXGIFactory), reinterpret_cast<void**>(dxgiFactory.GetAddressOf())), mGraphicsAdapters);
			ZE::ComPtr<IDXGIAdapter>  dxgiAdapter;
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
}
#endif
