// Copyright (C) Eser Kokturk. All Rights Reserved.

#include <Graphics/API/D3D/DebugInfoD3D.h>

#if ZE_GRAPHICS_D3D
#	if ZE_DEBUG

namespace Zeron::Gfx
{
	DebugInfoQueueD3D::DebugInfoQueueD3D()
		: mInfoQueue(nullptr)
		, mInfoFlags(DXGI_DEBUG_ALL)
		, mIsInitialized(false)
	{
		if (!mIsInitialized) {
			// https://walbourn.github.io/dxgi-debug-device/
			// Load dxgidubug library to get DXGIGetDebugInterface
			using LPDXGIGETDEBUGINTERFACE = HRESULT(WINAPI*)(REFIID, void**);
			const HMODULE dxgiDebug = LoadLibraryEx("dxgidebug.dll", nullptr, LOAD_LIBRARY_SEARCH_SYSTEM32);
			if (dxgiDebug) {
				auto dxgiGetDebugInterface = static_cast<LPDXGIGETDEBUGINTERFACE>(reinterpret_cast<void*>(GetProcAddress(dxgiDebug, "DXGIGetDebugInterface")));
				if (SUCCEEDED(dxgiGetDebugInterface(IID_PPV_ARGS(mInfoQueue.GetAddressOf())))) {
					mInfoFlags = DXGI_DEBUG_ALL;

					// -- If we need to breakpoint errors
					// dxgiInfoQueue->SetBreakOnSeverity(DXGI_DEBUG_ALL, DXGI_INFO_QUEUE_MESSAGE_SEVERITY_ERROR, true);
					// dxgiInfoQueue->SetBreakOnSeverity(DXGI_DEBUG_ALL, DXGI_INFO_QUEUE_MESSAGE_SEVERITY_CORRUPTION, true);
					mIsInitialized = true;
					return;
				}
				// FreeLibrary(dxgidebug);
				ZE_FAIL("Direct3D Debug Message Queue is not initialized properly!");
			}
			ZE_FAIL("DXGI debug library 'dxgidebug.dll' could not be loaded!");
		}
	}

	UINT64 DebugInfoQueueD3D::GetMessageCount() const
	{
		return mInfoQueue->GetNumStoredMessages(mInfoFlags);
	}

	std::string DebugInfoQueueD3D::GetMessages(UINT64 startIndex) const
	{
		const UINT64 end = mInfoQueue->GetNumStoredMessages(mInfoFlags);
		std::string messageOutput;
		for (auto i = startIndex; i < end; i++) {
			SIZE_T messagesLength;
			mInfoQueue->GetMessageA(mInfoFlags, i, nullptr, &messagesLength);
			auto bytes = std::make_unique<byte[]>(messagesLength);
			const auto pMessage = reinterpret_cast<DXGI_INFO_QUEUE_MESSAGE*>(bytes.get());
			mInfoQueue->GetMessageA(mInfoFlags, i, pMessage, &messagesLength);
			messageOutput.append(pMessage->pDescription);
			messageOutput.push_back('\n');
		}
		return messageOutput;
	}

	DebugInfoD3D::DebugInfoD3D()
		: mMessageIndex(D3DGlobalInfoQueue().GetMessageCount())
	{}

	std::string DebugInfoD3D::GetErrorMessage() const
	{
		return D3DGlobalInfoQueue().GetMessages(mMessageIndex);
	}

	std::string DebugInfoD3D::GetResultInfo(HRESULT result) const
	{
		std::stringstream description;
		description << "ERROR CODE: 0x" << std::hex << result << std::dec << " (" << result << ")\n";
		const _com_error err(result);
		description << "  DESCRIPTION: " << err.ErrorMessage();
		description << "\n  ERROR INFO: ";
		description << GetErrorMessage();
		return description.str();
	}
}
#	endif
#endif