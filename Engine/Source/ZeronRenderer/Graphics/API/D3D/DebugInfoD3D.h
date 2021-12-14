// Copyright (C) 2020, Eser Kokturk. All Rights Reserved.

#pragma once

#if ZE_GRAPHICS_D3D
#if ZE_DEBUG
#include <wrl.h>
#include <comdef.h>
#include <initguid.h>
#include <dxgidebug.h>

namespace Zeron {

	class DebugInfoQueueD3D {
	public:
		DebugInfoQueueD3D();
		DebugInfoQueueD3D(const DebugInfoQueueD3D&) = delete;
		DebugInfoQueueD3D & operator=(const DebugInfoQueueD3D&) = delete;

		[[nodiscard]] UINT64 GetMessageCount() const;
		[[nodiscard]] std::string GetMessages(UINT64 startIndex = 0) const;
		
	private:
		Microsoft::WRL::ComPtr<IDXGIInfoQueue> mInfoQueue;
		GUID mInfoFlags;
		bool mIsInitialized;
	};
	
	static DebugInfoQueueD3D& D3DGlobalInfoQueue() {
		static DebugInfoQueueD3D infoQueue;
		return infoQueue;
	}

	class DebugInfoD3D
	{
	public:
		DebugInfoD3D();
		[[nodiscard]] std::string GetInfo(HRESULT result) const;

	private:
		UINT64 mMessageIndex;
	};
}
#endif

#if ZE_DEBUG
#define D3D_CALL(call, return_type) \
do { \
	::Zeron::DebugInfoD3D debugInfo; \
	const HRESULT result = (call); \
	if (FAILED(result)){ \
		ZE_FAIL("{}", debugInfo.GetInfo(result).c_str()); \
		return return_type; \
	} \
} while (0)
#else
#define D3D_CALL(call, return_type) \
do { \
	const HRESULT result = (call); \
	if (FAILED(result)){ \
		ZE_LOGE("ERROR Direct3D: code {}", result); \
		return return_type;\
	} \
} while (0)
#endif

#endif
