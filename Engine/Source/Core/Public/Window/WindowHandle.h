// Copyright (C) 2020, Eser Kokturk. All Rights Reserved.

#pragma once

namespace ZeronEngine{

    class Window;
	class WindowModule;
	/*
	 * Handle for accessing Window more dynamically
	 * It takes in reference to the creator of the window (WindowModule) and id of the created window
	 * It can return a reference to the Window itself if it is valid
	 */
	struct WindowHandle
	{
		WindowHandle() : m_HandleID(0), m_WindowCreator(nullptr) {}
		WindowHandle(int handleID, const WindowModule* windowCreator) : m_HandleID(handleID), m_WindowCreator(windowCreator) {}
		WindowHandle(const WindowHandle& otherHandle) = default;
		WindowHandle(WindowHandle&& otherHandle) = default;
		
		bool IsValid() const;

		Window* Get() const;

		WindowHandle& operator=(const WindowHandle& other)
		{
			m_HandleID = other.m_HandleID;
			m_WindowCreator = other.m_WindowCreator;
			return *this;
		}
		
		bool operator==(const WindowHandle& Other) const
		{
			return Other.m_HandleID == m_HandleID && Other.m_WindowCreator == m_WindowCreator;
		}

		bool operator!=(const WindowHandle& Other) const
		{
			return (Other.m_HandleID == m_HandleID && Other.m_WindowCreator == m_WindowCreator) == false;
		}

		int GetHandleID() const { return m_HandleID; }

	private:
		int m_HandleID;
		const WindowModule* m_WindowCreator;

		friend class WindowModule;
	};

}