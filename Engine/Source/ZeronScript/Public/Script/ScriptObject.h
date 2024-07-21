// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once
#include <Script/TypeInfo.h>

namespace Zeron::Script
{
	struct ObjectInstance {
		using Handle = void*;
		using Owner = std::shared_ptr<void>;

		Handle mHandle = nullptr;
		const TypeInfo& mTypeInfo;
	};

	const ObjectInstance NullObject = { nullptr, TypeInfo() };
}