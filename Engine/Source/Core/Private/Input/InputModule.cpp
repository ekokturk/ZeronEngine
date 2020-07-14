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

		Dispatcher.Register<Events::Key::Press>(this, [](const Events::Key::Press& e)
		{
			ZERON_LOG("{} Pressed", e.Key.ToString())
		});

		Dispatcher.Register<Events::Key::Release>(this, [](const Events::Key::Release& e)
		{
			ZERON_LOG("{} Released", e.Key.ToString())
		});
		Dispatcher.Register<Events::Key::Repeat>(this, [](const Events::Key::Repeat& e)
		{
			ZERON_LOG("{} Repeat", e.Key.ToString())
		});
		Dispatcher.Register<Events::Key::Character>(this, [](const Events::Key::Character& e)
		{
			ZERON_LOG("\u{} Pressed", "\u"+e.UTF8)
		});

	}


}
