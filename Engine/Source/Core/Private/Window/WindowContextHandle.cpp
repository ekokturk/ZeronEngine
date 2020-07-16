// Copyright (C) 2020, Eser Kokturk. All Rights Reserved.

#include "Window/WindowContextHandle.h"
#include "Window/WindowModule.h"

namespace ZeronEngine{

	bool WindowContextHandle::IsValid() const
	{
		return m_WindowCreator && m_WindowCreator->HasWindow(*this);
	}

	WindowContext* WindowContextHandle::Get() const
	{
		if(IsValid())
		{
			return m_WindowCreator->GetWindow(m_HandleID);
		}
		return nullptr;
	}


}