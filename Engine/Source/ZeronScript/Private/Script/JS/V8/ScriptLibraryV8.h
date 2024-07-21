// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

#include <Script/ScriptLibrary.h>

namespace v8
{
	class Platform;
	class Isolate;
}

namespace Zeron::Script
{

	class LibraryV8 final : public Library {
	  public:
		LibraryV8();
		~LibraryV8() override;

		std::unique_ptr<Runtime> CreateRuntime() override;

	  private:
		// V8 Library resources cannot be re-initialized within the same process
		// This handle manages resource lifetime statically
		struct PlatformHandle {
			PlatformHandle();
			~PlatformHandle();
			std::unique_ptr<v8::Platform> mPlatform;
		};

		v8::Platform* mPlatform = nullptr;
	};


}