// Copyright (C) 2020, Eser Kokturk. All Rights Reserved.

#pragma once
#include <functional>
#include <string>


namespace ZeronEngine
{
	class Event;
	
	class Window
	{
	private:
		int m_Width;
		int m_Height;
		std::string m_Name;

		std::function<void(Event&)> m_EventCallback;
		
	public:
		Window();
		~Window();
		
		void Init();
		void Update();
		void Destroy();

		int GetWidth() const { return m_Width; }
		int GetHeight() const { return m_Height; }
		std::string GetName() const { return m_Name; }

		void SetEventCallback(std::function<void(Event&)>& callback);

	};
}
