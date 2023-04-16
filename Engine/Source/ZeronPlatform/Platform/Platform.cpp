// Copyright (C) Eser Kokturk. All Rights Reserved.

#include <Platform/Platform.h>

#include <Platform/SystemEvent.h>
#include <Platform/SystemEventProcessor.h>
#include <Platform/Window.h>
#include <Platform/WindowManager.h>

#include <Platform/FileSystem.h>
#include <Platform/API/Shared/FileSystemHandlerStandard.h>

namespace Zeron
{
	Platform::Platform()
	{
		mWindowManager = std::make_unique<WindowManager>([this](const auto& evt, const auto& ctx) {
			_dispatchEvents(evt, ctx);
		});
	}

	Platform::~Platform()
	{
	}

	void Platform::_dispatchEvents(const SystemEvent& evt, const SystemEvent::Context& ctx)
	{
	}

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
}
