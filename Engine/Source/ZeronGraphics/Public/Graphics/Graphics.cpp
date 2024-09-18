// Copyright (C) Eser Kokturk. All Rights Reserved.

#include <Graphics/Graphics.h>

#include <Graphics/API/D3D/11/GraphicsD3D11.h>
#include <Graphics/API/Vulkan/GraphicsVulkan.h>
#include <Graphics/Buffer.h>

namespace Zeron::Gfx
{
	std::unique_ptr<Graphics> Graphics::CreateGraphics(GraphicsType api)
	{
		switch (api) {
#if ZE_GRAPHICS_D3D
			case GraphicsType::Direct3D11: return std::make_unique<GraphicsD3D11>();
#endif
#if ZE_GRAPHICS_VULKAN
			case GraphicsType::Vulkan: return std::make_unique<GraphicsVulkan>();
#endif
			case GraphicsType::Null:;
			default: ZE_FAIL("Graphics API is not supported!");
		}
		return nullptr;
	}

	std::unique_ptr<Buffer> Graphics::CreateIndexBuffer(const std::vector<uint32_t>& data)
	{
		return CreateBuffer(BufferType::Index, static_cast<uint32_t>(data.size()), sizeof(uint32_t), &data.front());
	}
}