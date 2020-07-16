// Copyright (C) 2020, Eser Kokturk. All Rights Reserved.

#include "Window/WindowHandle.h"
#include "Window/WindowModule.h"

namespace ZeronEngine{

	bool WindowHandle::IsValid() const
	{
		return m_WindowCreator && m_WindowCreator->HasWindow(*this);
	}

	Window* WindowHandle::Get() const
	{
		if(IsValid())
		{
			return m_WindowCreator->GetWindow(m_HandleID);
		}
		return nullptr;
	}


}