// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

#if ZE_PLATFORM_WIN32

#	include <Platform/SystemEventProcessor.h>

namespace Zeron
{
	class WindowWin32;
	class Platform;

	class SystemEventProcessorWin32 : public SystemEventProcessor {
	  public:
		SystemEventProcessorWin32(DispatchFn dispatchCallback, ContextFn contextCallback);

		void ProcessEvents() override;

		static LRESULT CALLBACK WndProcRouter(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
		LRESULT WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	};
}

#endif