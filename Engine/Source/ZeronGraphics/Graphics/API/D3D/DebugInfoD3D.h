// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

#if ZE_GRAPHICS_D3D
#	if ZE_DEBUG

#		include <initguid.h>

#		include <dxgidebug.h>
#		include <Graphics/API/D3D/DXGI.h>

namespace Zeron::Gfx
{
	class DebugInfoQueueD3D {
	  public:
		DebugInfoQueueD3D();
		DebugInfoQueueD3D(const DebugInfoQueueD3D&) = delete;
		DebugInfoQueueD3D& operator=(const DebugInfoQueueD3D&) = delete;

		UINT64 GetMessageCount() const;
		std::string GetMessages(UINT64 startIndex = 0) const;

	  private:
		Gfx::ComPtr<IDXGIInfoQueue> mInfoQueue;
		GUID mInfoFlags;
		bool mIsInitialized;
	};

	static DebugInfoQueueD3D& D3DGlobalInfoQueue()
	{
		static DebugInfoQueueD3D infoQueue;
		return infoQueue;
	}

	class DebugInfoD3D {
	  public:
		DebugInfoD3D();
		std::string GetResultInfo(HRESULT result) const;
		std::string GetErrorMessage() const;

	  private:
		UINT64 mMessageIndex;
	};
}
#	endif

#	if ZE_DEBUG
#		define ZE_D3D_ASSERT(call)                                       \
			do {                                                          \
				::Zeron::Gfx::DebugInfoD3D debugInfo;                     \
				(call);                                                   \
				const std::string errorMsg = debugInfo.GetErrorMessage(); \
				if (!errorMsg.empty()) {                                  \
					ZE_FAIL("ERROR INFO: {}", errorMsg.c_str());          \
				}                                                         \
			}                                                             \
			while (0)

#		define ZE_D3D_ASSERT_RESULT(call, ...)                             \
			do {                                                            \
				::Zeron::Gfx::DebugInfoD3D debugInfo;                       \
				const HRESULT result = (call);                              \
				if (FAILED(result)) {                                       \
					ZE_FAIL("{}", debugInfo.GetResultInfo(result).c_str()); \
					return __VA_ARGS__;                                     \
				}                                                           \
			}                                                               \
			while (0)
#	else
#		define ZE_D3D_ASSERT(call) (call)
#		define ZE_D3D_ASSERT_RESULT(call, ...)                 \
			do {                                                \
				const HRESULT result = (call);                  \
				if (FAILED(result)) {                           \
					ZE_LOGE("ERROR Direct3D: code {}", result); \
					return __VA_ARGS__;                         \
				}                                               \
			}                                                   \
			while (0)
#	endif

#endif