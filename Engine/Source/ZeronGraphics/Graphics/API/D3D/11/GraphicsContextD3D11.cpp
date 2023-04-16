// Copyright (C) Eser Kokturk. All Rights Reserved.


#ifdef ZE_GRAPHICS_D3D

#	include <Graphics/API/D3D/11/GraphicsContextD3D11.h>

#	include <Graphics/API/D3D/11/BufferD3D11.h>
#	include <Graphics/API/D3D/11/CommandBufferD3D11.h>
#	include <Graphics/API/D3D/11/FrameBufferD3D11.h>
#	include <Graphics/API/D3D/11/GraphicsD3D11.h>
#	include <Graphics/API/D3D/11/ShaderD3D11.h>
#	include <Graphics/API/D3D/11/SwapChainD3D11.h>
#	include <Graphics/API/D3D/11/TextureD3D11.h>
#	include <Graphics/API/D3D/DebugInfoD3D.h>

namespace Zeron
{

	GraphicsContextD3D11::GraphicsContextD3D11(GraphicsD3D11& graphics)
		: mGraphics(graphics)
		, mDevice(graphics.GetDeviceD3D())
	{
		// TODO: Deffered Context
		mDeviceContext = graphics.GetDeviceContextD3D();
	}

	void GraphicsContextD3D11::Init(SystemHandle systemHandle, const Vec2i& size)
	{
		ZE_ASSERT(!mSwapChain, "Vulkan graphics context swapchain was already initialized!");
		mSwapChain = std::make_unique<SwapChainD3D11>(mGraphics, systemHandle, size);
		mCommandBuffer = std::make_unique<CommandBufferD3D11>(mGraphics);
	}

	CommandBuffer& GraphicsContextD3D11::BeginCommands()
	{
		return *mCommandBuffer;
	}

	void GraphicsContextD3D11::EndCommands() {}

	void GraphicsContextD3D11::Submit(CommandBuffer& cmd) {}

	void GraphicsContextD3D11::BeginSwapChainRenderPass(CommandBuffer& cmd) const
	{
		cmd.BeginRenderPass(mSwapChain->GetFrameBuffer());
	}

	void GraphicsContextD3D11::EndSwapChainRenderPass(CommandBuffer& cmd) const {}

	void GraphicsContextD3D11::Present()
	{
		mSwapChain->Present();
	}

	void GraphicsContextD3D11::ResizeSwapChain(const Vec2i& size)
	{
		if (auto* frameBuffer = static_cast<FrameBufferD3D11*>(mSwapChain->GetFrameBuffer())) {
			frameBuffer->ReleaseBuffers();
			mSwapChain->Resize(size);
			ZE_D3D_ASSERT(frameBuffer->CreateBuffers(mDevice, *mSwapChain, mGraphics.GetMultiSamplingLevel()));
		}
	}

	SwapChain* GraphicsContextD3D11::GetSwapChain() const
	{
		return mSwapChain.get();
	}

	RenderPass* GraphicsContextD3D11::GetSwapChainRenderPass() const
	{
		return nullptr;
	}

	const Vec2i& GraphicsContextD3D11::GetSwapChainSize() const
	{
		return mSwapChain->GetSize();
	}

	uint32_t GraphicsContextD3D11::GetMaxFramesInFlight() const
	{
		return 1;
	}
}
#endif