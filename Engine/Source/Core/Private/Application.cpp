// Copyright (C) 2020, Eser Kokturk. All Rights Reserved.

#include "../Public/Application.h"


namespace ZeronEngine
{
	Application * Application::s_Instance = nullptr;
	
	Application::Application()
	{
		s_Instance = this;
	}

	Application* Application::GetInstance()
	{
		return s_Instance;
	}

	void Application::Destroy()
	{
		if(s_Instance != nullptr)
		{
			delete s_Instance;
			s_Instance = nullptr;
		}
	}
}
