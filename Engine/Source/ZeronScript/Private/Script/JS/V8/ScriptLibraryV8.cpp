// Copyright (C) Eser Kokturk. All Rights Reserved.

#include <Script/JS/V8/ScriptLibraryV8.h>

#include <libplatform/libplatform.h>
#include <Script/JS/V8/ScriptRuntimeV8.h>
#include <v8-initialization.h>


namespace Zeron::Script
{
	LibraryV8::LibraryV8()
	{
		static PlatformHandle resource;
		mPlatform = resource.mPlatform.get();
	}

	LibraryV8::~LibraryV8() {}

	std::unique_ptr<Runtime> LibraryV8::CreateRuntime()
	{
		ZE_ASSERT(mPlatform, "Expected V8 Library to be initialized!");
		return std::make_unique<RuntimeV8>(*mPlatform);
	}

	LibraryV8::PlatformHandle::PlatformHandle()
	{
		v8::V8::InitializeICUDefaultLocation("./");
		v8::V8::InitializeExternalStartupData("./");
		mPlatform = v8::platform::NewDefaultPlatform();
		v8::V8::InitializePlatform(mPlatform.get());
		v8::V8::Initialize();
	}

	LibraryV8::PlatformHandle::~PlatformHandle()
	{
		v8::V8::Dispose();
		v8::V8::DisposePlatform();
	}

}