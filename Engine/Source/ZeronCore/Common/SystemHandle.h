// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

namespace Zeron {

	// Handle for passing around OS information
	class SystemHandle {
	public:
#if ZE_PLATFORM_WIN32
		void* mWindow = nullptr;
		bool operator ==(const SystemHandle& other) const { return mWindow == other.mWindow; }
#elif ZE_PLATFORM_LINUX
		unsigned int mWindow = 0;
		void* mConnection = nullptr;
		bool operator ==(const SystemHandle& other) const { return mWindow == other.mWindow && mConnection == other.mConnection; }
#elif ZE_PLATFORM_ANDROID
		void* mWindow = nullptr;
		bool operator ==(const SystemHandle& other) const { return mWindow == other.mWindow; }
#endif
	};

}
