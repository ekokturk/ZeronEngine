// Copyright (C) Eser Kokturk. All Rights Reserved.

#if ZE_PLATFORM_ANDROID

#	include <Platform/API/Android/SystemEventProcessorAndroid.h>

#	include <Core/SystemHandle.h>
#	include <Core/SystemEvent.h>
#	include <Platform/Window.h>
#	include <Platform/WindowManager.h>


namespace Zeron
{
	SystemEventProcessorAndroid::SystemEventProcessorAndroid(DispatchFn dispatchCallback, ContextFn contextCallback)
		: SystemEventProcessor(std::move(dispatchCallback), std::move(contextCallback))
	{}

	void SystemEventProcessorAndroid::ProcessEvents() {}
}

#endif