// Copyright (C) 2020, Eser Kokturk. All Rights Reserved.

#pragma once
#include "GraphicsTypes.h"
#include "Primitives.h"

namespace Zeron
{
	class Buffer;
	class Texture;
	class Shader;
	class GraphicsContext;
	class Window;
	class SwapChain;

	class Graphics
	{
	public:
		virtual ~Graphics() = default;
		virtual bool Init() = 0;

		virtual GraphicsType GetGraphicsType() const = 0;

		virtual std::shared_ptr<GraphicsContext> GetImmediateContext() const = 0;
		
		virtual std::shared_ptr<GraphicsContext> CreateGraphicsContext() = 0;
		virtual std::shared_ptr<SwapChain> CreateSwapChain(Window& window) = 0;
		template <typename T>
		std::shared_ptr<Buffer> CreateVertexBuffer(const std::vector<T>& data) {
			return CreateBuffer(BufferType::Vertex,  &data.front(), static_cast<uint32_t>(data.size()), sizeof(T));
		}
		std::shared_ptr<Buffer> CreateIndexBuffer(const std::vector<unsigned long>& data);
		template <typename T>
		std::shared_ptr<Buffer> CreateConstantBuffer(const T& data) {
			return CreateBuffer(BufferType::Constant,  &data, 1, sizeof(T));
		}
		virtual std::shared_ptr<Shader> CreateShader(const std::string& name) = 0;
		
		virtual std::shared_ptr<Texture> CreateTexture(TextureType type, const Color& data) = 0;
		virtual std::shared_ptr<Texture> CreateTexture(TextureType type, const std::string& path) = 0;
		virtual std::shared_ptr<Texture> CreateTexture(TextureType type, const Color* data, uint32_t width, uint32_t height) = 0;
	
	protected:
		virtual std::shared_ptr<Buffer> CreateBuffer(BufferType type, const void* data, uint32_t count, uint32_t stride) = 0;
	};
}
