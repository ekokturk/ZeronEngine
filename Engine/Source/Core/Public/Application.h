// Copyright (C) 2020, Eser Kokturk. All Rights Reserved.

#pragma once

#include "Core.h"
#include "Logger.h"

// Create derived Zeron application
#define GENERATE_ZERON_APPLICATION(AppClass)\
		ZeronEngine::Application* ZeronEngine::CreateApplication()\
		{\
			return new AppClass();\
		}\
		\
		int main(int argc, char** argv)\
		{\
			{ZeronEngine::Logger::Init();}\
			ZeronEngine::CreateApplication();\
			AppClass::GetInstance()->Run();\
			AppClass::GetInstance()->Destroy();\
		}\

namespace ZeronEngine
{
	class Application
	{
	private:
		static Application * s_Instance;
	public:
		Application();

		static Application * GetInstance();
		
		virtual ~Application(){}
		virtual void Run() {}
		void Destroy();
	};

	Application * CreateApplication();
}
