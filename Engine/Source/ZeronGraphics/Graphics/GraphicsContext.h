// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

#include <Graphics/GraphicsTypes.h>

namespace Zeron::Gfx
{
	class CommandBuffer;
	class RenderPass;
	class ShaderProgram;
	class Buffer;
	class FrameBuffer;
	class Shader;
	class SwapChain;
	class Texture;
	class Window;

	class GraphicsContext {
	  public:
		virtual ~GraphicsContext() = default;

		virtual void Init(SystemHandle systemHandle, const Vec2i& size) = 0;
		virtual CommandBuffer& BeginCommands() = 0;
		virtual void EndCommands() = 0;

		virtual void Submit(CommandBuffer& cmd) = 0;
		virtual void BeginSwapChainRenderPass(CommandBuffer& cmd) const = 0;
		virtual void EndSwapChainRenderPass(CommandBuffer& cmd) const = 0;

		virtual void Present() = 0;
		virtual void ResizeSwapChain(const Vec2i& size) = 0;

		virtual SwapChain* GetSwapChain() const = 0;
		virtual RenderPass* GetSwapChainRenderPass() const = 0;
		virtual const Vec2i& GetSwapChainSize() const = 0;

		virtual uint32_t GetMaxFramesInFlight() const = 0;
	};

}