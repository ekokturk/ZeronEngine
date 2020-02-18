// Copyright (C) 2020, Eser Kokturk. All Rights Reserved.

#pragma once

#include "Core.h"
#include <string>
#include <iostream>

// Create derieved Zeron application
#define CREATE_ZERON_APP(AppClass)\
		ZeronEngine::Application* ZeronEngine::CreateApplication()\
		{\
		return new AppClass();\
		}\

namespace ZeronEngine
{
	class ZRN_API Application
	{
	private:
		static Application * m_Instance;
	public:
		Application(){};
		
		virtual ~Application(){}
		virtual void Run() {}

	};

	Application * CreateApplication();
}
