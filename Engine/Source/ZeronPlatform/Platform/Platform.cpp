// Copyright (C) Eser Kokturk. All Rights Reserved.

#include <Platform/Platform.h>

#include <Platform/API/Shared/FileSystemHandlerStandard.h>
#include <Platform/FileSystem.h>
#include <Platform/SystemEvent.h>
#include <Platform/SystemEventProcessor.h>
#include <Platform/Window.h>
#include <Platform/WindowManager.h>

namespace Zeron
{
	Platform::Platform()
	{
		mWindowManager = std::make_unique<WindowManager>([this](const auto& evt, const auto& ctx) {
			_dispatchEvents(evt, ctx);
		});
	}

	Platform::~Platform() {}

	bool Platform::Init()
	{
		// TODO: Move this to implementation
		mFileSystem = std::make_unique<FileSystemHandlerStandard>();
		return true;
	}

	void Platform::Update()
	{
		mWindowManager->Update();
	}

	bool Platform::IsExiting() const
	{
		return false;
	}

	IFileSystemHandler& Platform::GetFileSystem()
	{
		return *mFileSystem;
	}

	const IFileSystemHandler& Platform::GetFileSystem() const
	{
		return *mFileSystem;
	}

	Window* Platform::GetMainWindow() const
	{
		return mWindowManager->GetMainWindow();
	}

	WindowManager* Platform::GetWindowManager() const
	{
		return mWindowManager.get();
	}

	void Platform::_dispatchEvents(const SystemEvent& evt, const SystemEvent::Context& ctx) {}

	ByteBuffer Platform::_readWindowIcon() const
	{
		ZE_ASSERT(mFileSystem, "Expected file system to be initialized!");
		auto iconResult = mFileSystem->ReadBinaryFile("Resources/window_icon.png");
		if (!iconResult.HasError()) {
			return iconResult.Value();
		}
		return {};
	}
}