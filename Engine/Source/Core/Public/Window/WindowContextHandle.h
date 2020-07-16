// Copyright (C) 2020, Eser Kokturk. All Rights Reserved.

#pragma once

namespace ZeronEngine{

    class WindowContext;
	class WindowModule;
	/*
	 * Handle for accessing window context more dynamically
	 * It takes in reference to the creator of the window (WindowModule) and id of the created window
	 * It can return a reference to the window context itself if it is valid
	 */
	struct WindowContextHandle
	{
		WindowContextHandle() : m_HandleID(0), m_WindowCreator(nullptr) {}
		WindowContextHandle(int handleID, const WindowModule* windowCreator) : m_HandleID(handleID), m_WindowCreator(windowCreator) {}
		WindowContextHandle(const WindowContextHandle& otherHandle) = default;
		WindowContextHandle(WindowContextHandle&& otherHandle) = default;
		
		bool IsValid() const;

		WindowContext* Get() const;

		WindowContextHandle& operator=(const WindowContextHandle& other)
		{
			m_HandleID = other.m_HandleID;
			m_WindowCreator = other.m_WindowCreator;
			return *this;
		}
		
		bool operator==(const WindowContextHandle& Other) const
		{
			return Other.m_HandleID == m_HandleID && Other.m_WindowCreator == m_WindowCreator;
		}

		bool operator!=(const WindowContextHandle& Other) const
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