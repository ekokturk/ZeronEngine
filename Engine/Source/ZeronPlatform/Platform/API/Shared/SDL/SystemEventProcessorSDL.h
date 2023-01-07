// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

#if ZE_WINDOW_SDL

#include <Platform/SystemEventProcessor.h>

struct SDL_Window;

namespace Zeron
{
	class WindowSDL;
	class Platform;

	class SystemEventProcessorSDL : public SystemEventProcessor {
	public:
		SystemEventProcessorSDL(DispatchFn dispatchCallback, ContextFn contextCallback);

		void ProcessEvents() override;
	};
}

#endif