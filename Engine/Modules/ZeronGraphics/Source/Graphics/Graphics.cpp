// Copyright (C) Eser Kokturk. All Rights Reserved.

#include <Graphics/Graphics.h>

namespace Zeron
{
	std::shared_ptr<Buffer> Graphics::CreateIndexBuffer(const std::vector<unsigned long>& data)
	{
		return CreateBuffer(BufferType::Index, &data.front(), static_cast<uint32_t>(data.size()), sizeof(unsigned long));
	}
}
