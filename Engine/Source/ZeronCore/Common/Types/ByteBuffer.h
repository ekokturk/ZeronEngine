// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

namespace Zeron
{
	class ByteBuffer : public std::vector<std::byte> {
		using BaseType = std::vector<std::byte>;
		using BaseType::BaseType;
	};
}
