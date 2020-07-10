// Copyright (C) 2020, Eser Kokturk. All Rights Reserved.

#include "../Public/Input.h"



#include "Application.h"
#include "Logger.h"
#include "Events/EventDispatcher.h"
#include "Events/EventTypes.h"

namespace ZeronEngine
{
	void Input::RegisterEvents(EventDispatcher& Dispatcher)
	{
		//auto eventCallback = [=](Events::Window_Closed& e)
		//{
		//	//ZERON_LOG_ERROR("PosX:{} Pos:{}", e.PosX, e.PosY)
		//		//e.Consume();
		//};

		//Dispatcher.Register<Events::Window_Closed>(this, eventCallback);
	}
}
