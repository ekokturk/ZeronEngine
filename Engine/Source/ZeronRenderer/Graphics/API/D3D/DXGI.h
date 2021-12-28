// Copyright (C) 2020, Eser Kokturk. All Rights Reserved.

#pragma once

#include <wrl.h>
#include <comdef.h>
#include <dxgi.h>

namespace Zeron {
	namespace ZE {
		template <typename T>
		using ComPtr = ::Microsoft::WRL::ComPtr<T>;
	}
}
