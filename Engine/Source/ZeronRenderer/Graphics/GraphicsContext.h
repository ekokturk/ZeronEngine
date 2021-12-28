// Copyright (C) 2020, Eser Kokturk. All Rights Reserved.

#pragma once
#include "GraphicsTypes.h"

namespace Zeron
{
	class Buffer;
	class Texture;
	class Shader;
	class RenderTarget;
	class Window;
	
	class GraphicsContext {
	public:
		virtual ~GraphicsContext() = default;

		virtual void SetRenderTarget(RenderTarget* target) = 0;
		virtual void SetShader(Shader* shader) = 0;
		virtual void SetPrimitiveTopology(PrimitiveTopology topology) = 0;
		virtual void SetTexture(Texture* texture) = 0;
		virtual void SetVertexBuffer(Buffer& vb) = 0;
		virtual void SetIndexBuffer(Buffer& ib) = 0;
		virtual void SetConstantBuffer(Buffer& cb) = 0;

		virtual void SetFillMode(bool isSolid) = 0;
		virtual void UpdateBuffer(Buffer& buff, void* data, uint32_t size) = 0;

		virtual void DrawIndexed(uint32_t indexCount, uint32_t indexStart) = 0;
		virtual void Clear(Color color) = 0;


	private:
	};

}
