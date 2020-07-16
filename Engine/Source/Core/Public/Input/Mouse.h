// Copyright (C) 2020, Eser Kokturk. All Rights Reserved.

#pragma once
#include "CoreTypes/Vector2.h"
#include "Input/InputDevice.h"

namespace ZeronEngine
{

	class Mouse : public InputDevice
	{
	public:
		
		Mouse();
		~Mouse();


		void BindMouseInput(MouseCode mouseCode, MouseInputState inputState, std::function<void()> callback)
		{
		}
		
		void Broadcast(MouseCode mouseCode)
		{
		}


		//Vector2 GetMousePosition() const {};
		//Vector2 GetMouseScreenPosition() const {};
		//MouseInputState GetMouseButtonState(MouseCode button) const{};

	private:


	};

}

