// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

#include <comdef.h>
#include <dxgi.h>
#include <wrl.h>

namespace Zeron::Gfx
{
	template <typename T>
	using ComPtr = ::Microsoft::WRL::ComPtr<T>;
}