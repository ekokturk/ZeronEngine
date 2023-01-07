// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

#if ZE_GRAPHICS_D3D
#include <d3d11.h>
#include <Graphics/API/D3D/DXGI.h>
#include <Graphics/GraphicsContext.h>

struct ID3D11DepthStencilState;
struct ID3D11Device;
struct ID3D11DeviceContext;
struct ID3D11RasterizerState;
struct ID3D11SamplerState;

namespace Zeron
{
	class CommandBufferD3D11;
	class SwapChainD3D11;
	class Buffer;
	class GraphicsD3D11;
	class FrameBufferD3D11;
	class Shader;
	class Texture;

	class GraphicsContextD3D11 final : public GraphicsContext {
	public:
		GraphicsContextD3D11(GraphicsD3D11& graphics);
		~GraphicsContextD3D11() = default;

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
		GraphicsD3D11& mGraphics;

		std::unique_ptr<SwapChainD3D11> mSwapChain;
		std::unique_ptr<CommandBufferD3D11> mCommandBuffer;

		ID3D11Device* mDevice;
		ID3D11DeviceContext* mDeviceContext;
		ZE::ComPtr<ID3D11DeviceContext> mDefferedContext;
	};
}
#endif
