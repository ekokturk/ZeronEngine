// Copyright (C) 2020, Eser Kokturk. All Rights Reserved.

#include "Input/InputModule.h"

#include "Logger.h"
#include "Events/EventTypes/EventTypes.h"
#include "Events/EventDispatcher.h"
#include "Input/InputGLFW.h"

namespace ZeronEngine
{
	void InputModule::Init()
	{
		SetupInputClass<InputGLFW>();
	}

	void InputModule::Update()
	{
		
	}

	void InputModule::Destroy()
	{
	}
	
	void InputModule::RegisterEvents(const EventDispatcher& Dispatcher)
	{

		Dispatcher.Register<Events::Mouse::Press>(this, [](const Events::Mouse::Press& e)
		{
			ZERON_LOG("{} Pressed", e.Button.ToString())
		});

		Dispatcher.Register<Events::Mouse::Release>(this, [](const Events::Mouse::Release& e)
		{
			ZERON_LOG("{} Released", e.Button.ToString())
		});
	}


}
