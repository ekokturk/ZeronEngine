// Copyright (C) 2020, Eser Kokturk. All Rights Reserved.

#pragma once

#include "InputMapping.h"
#include "Mouse.h"
#include "CoreTypes/Vector2.h"
#include "Window/WindowContextHandle.h"


namespace ZeronEngine
{
	class EventDispatcher;

	class InputModule 
	{
		
	public:
		InputModule();
		~InputModule();

		void Init();
		void Update();
		void Destroy();

		// Set input mapping 
		void SetInputMapping(std::shared_ptr<InputMapping> newInputMapping);


		// MOUSE INPUTS

		bool BindInput(MouseCode mouseCode, MouseInputState inputState, const std::function<void()>& callback);

		bool BindInput(const std::string& mouseEvent, MouseInputState inputState, const std::function<void()>& callback);

		void RegisterEvents(const EventDispatcher& Dispatcher);

		Vector2 GetMousePosition() const;
		
	private:

		std::shared_ptr<InputMapping> m_CurrentInputMapping;
		std::unique_ptr<Mouse> m_Mouse;


		// Window contexts for input reference
		WindowContextHandle m_FocusedWindow;
		WindowContextHandle m_HoveredWindow;
	};
}
