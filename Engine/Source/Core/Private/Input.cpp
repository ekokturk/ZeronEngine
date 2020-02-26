// Copyright (C) 2020, Eser Kokturk. All Rights Reserved.

#include "../Public/Input.h"

namespace ZeronEngine
{
	void Input::SetEventCallback(std::function<void(Event&)>& callback)
	{
		m_EventCallback = callback;
	}

}
