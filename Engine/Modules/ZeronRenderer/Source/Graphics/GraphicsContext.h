// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once
#include "GraphicsTypes.h"

namespace Zeron
{
	class ShaderProgram;
	class Buffer;
	class RenderTarget;
	class Shader;
	class SwapChain;
	class Texture;
	class Window;
	
	class GraphicsContext {
	public:
		virtual ~GraphicsContext() = default;

		virtual void SetRenderTarget(RenderTarget* target) = 0;
		virtual void SetShader(Shader* shader) = 0;
		virtual void SetShaderProgram(ShaderProgram* shader) = 0;
		virtual void SetPrimitiveTopology(PrimitiveTopology topology) = 0;
		virtual void SetTexture(Texture* texture, uint8_t slot = 0) = 0;
		virtual void SetVertexBuffer(Buffer& vb, uint32_t slot = 0) = 0;
		virtual void SetVertexBuffers(Buffer** vb, uint32_t count, uint32_t slot = 0) = 0;
		virtual void SetIndexBuffer(Buffer& ib) = 0;
		virtual void SetConstantBuffer(Buffer& cb, ShaderType type, uint32_t slot = 0) = 0;

		virtual void SetFillMode(bool isSolid) = 0;
		virtual void UpdateBuffer(Buffer& buff, void* data, uint32_t sizeBytes) = 0;

		virtual void Draw(uint32_t vertexCount, uint32_t vertexStart) = 0;
		virtual void DrawIndexed(uint32_t indexCount, uint32_t indexStart) = 0;
		virtual void DrawInstanced(uint32_t vertexCount, uint32_t instanceCount, uint32_t vertexStart = 0, uint32_t instanceStart = 0) = 0;
		virtual void DrawInstancedIndexed(uint32_t indexCount, uint32_t instanceCount, uint32_t indexStart = 0, uint32_t baseVertexLocation = 0, uint32_t instanceStart = 0) = 0;
		virtual void Clear(Color color) = 0;

		virtual void ResizeSwapChain(SwapChain& swapChain, const Vec2i& size) = 0;
	};

}
