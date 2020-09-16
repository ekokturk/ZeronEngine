// Copyright (C) 2020, Eser Kokturk. All Rights Reserved.

#pragma once

#include "InputMapping.h"
#include "CoreTypes/Vector2.h"
#include "Window/WindowHandle.h"


namespace ZeronEngine
{
	struct pairhash {
		template <typename T, typename U>
		std::size_t operator()(const std::pair<T, U>& x) const
		{
			return std::hash<T>()(x.first) ^ std::hash<U>()(x.second);
		}
	};
	
	using InputActionCallback = std::pair<void*, std::function<void()>>;
	using InputPollCallback = std::pair<void*, std::function<void(float)>>;
	
	class EventDispatcher;

	class InputModule 
	{
		
	public:
		InputModule();
		~InputModule();

		void Init();
		void Update();
		void Destroy();

		// Set input mapping to be used for input bindings
		void SetInputMapping(std::shared_ptr<InputMapping> newInputMapping);


		// TODO use object pointer as a handle

		bool BindActionInput(void* owner, const std::string& inputName, InputState inputState, const InputActionCallback& callback);

		bool BindPollInput(const std::string& inputName, const InputPollCallback& callback);

		bool UnbindActionInput(const std::string& inputName);

		bool UnbindPollInput(const std::string& inputName);

		void RegisterEvents(const EventDispatcher& Dispatcher);

		Vector2 GetMousePosition() const;
		
	private:



		std::unordered_map<std::pair<InputActionBinding, InputState>, std::vector<InputActionCallback>, pairhash> m_ActionCallbacks;
		std::unordered_map < InputPollBinding, std::vector<InputPollCallback>> m_PollCallbacks;
		

		// Input mapping that is used to check for assigned 
		std::shared_ptr<InputMapping> m_CurrentInputMapping;

		
		// Window that is currently focused
		WindowHandle m_FocusedWindow;
		// Window that mouse is hovered on
		WindowHandle m_HoveredWindow;
	};


}
