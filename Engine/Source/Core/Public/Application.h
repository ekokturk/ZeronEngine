// Copyright (C) 2020, Eser Kokturk. All Rights Reserved.

#pragma once

#include "Core.h"
#include "Events/EventDispatcher.h"
#include "Events/IEventHandler.h"
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
	class Application: public IEventHandler
	{
	private:
		static Application * s_Instance;
		std::unique_ptr<EventDispatcher> m_EventDispatcher;
	public:
		Application();

		static Application * GetInstance();
		
		virtual ~Application();
		void Run();
		void Destroy();
		
		void OnEvent(const Event& e);
	};

	Application * CreateApplication();
}
