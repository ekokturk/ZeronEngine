// Copyright (C) 2020, Eser Kokturk. All Rights Reserved.

#pragma once


/*
 * Window abstract class 
 */

namespace ZeronEngine
{
	class EventDispatcher;
	class WindowContext;

	/*
	 * Properties for window context creation
	 */
	
	struct WindowProps
	{
		WindowProps(const EventDispatcher* dispatcher = nullptr);
		WindowProps(std::string& name, int width, int height, const EventDispatcher* dispatcher);
		
		std::string Name;
		int Width;
		int Height;
		const EventDispatcher* EventDispatcher;
	};

	struct WindowContextHandle
	{
		WindowContextHandle() : WindowRef(nullptr) {}
		WindowContextHandle(WindowContext& windowContext) : WindowRef(&windowContext) {}
		WindowContextHandle(WindowContext* windowContext) : WindowRef(windowContext) {}

		bool IsValid() const
		{
			return WindowRef != nullptr;
		}

		bool operator==(const WindowContextHandle& Other) const
		{
			return Other.WindowRef == WindowRef && WindowRef;
		}
		
		WindowContext* WindowRef;

	};
	
	
	class WindowContext 
	{
	public:

		/*
		 * Initialize context for the window that will be created
		 * Library method calls and initializations for all windows of context type can be done here
		 */
		template<typename T>
		static void InitContext();

		virtual void Update() = 0;
		virtual void Destroy() = 0;
		virtual void RegisterEvents() = 0;

		//WindowContextHandle MakeWindowContextHandle() const;
		
		int GetWidth() const { return m_WindowProps.Width; }
		int GetHeight() const { return m_WindowProps.Height; }
		const std::string& GetName() const { return m_WindowProps.Name; }

	public:
		virtual ~WindowContext();
		
	protected:
		WindowContext(const WindowProps& windowProps);
		WindowContext(WindowProps&& windowProps);

		
	protected:
		WindowProps m_WindowProps;
	};

	/*
	* Handle for window context accessors
	*/


}


namespace std
{
	template <>
	struct hash<ZeronEngine::WindowContextHandle>
	{
		size_t operator()(const ZeronEngine::WindowContextHandle& k) const
		{
			return hash<ZeronEngine::WindowContext*>()(k.WindowRef);
		}
	};
}