// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

#if ZE_PLATFORM_ANDROID

#include <Platform/SystemEventProcessor.h>

namespace Zeron
{
	class WindowAndroid;
	class Platform;

	class SystemEventProcessorAndroid : public SystemEventProcessor {
	public:
		SystemEventProcessorAndroid(DispatchFn dispatchCallback, ContextFn contextCallback);

		void ProcessEvents() override;

	};
}

#endif