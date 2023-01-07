// Copyright (C) Eser Kokturk. All Rights Reserved.

#if ZE_WINDOW_SDL

#include <Platform/API/Shared/SDL/SystemEventProcessorSDL.h>

#include <SDL_events.h>
#include <Platform/API/Shared/SDL/SDLHelpers.h>
#include <Platform/API/Shared/SDL/WindowSDL.h>

namespace Zeron
{
	SystemEventProcessorSDL::SystemEventProcessorSDL(DispatchFn dispatchCallback, ContextFn contextCallback)
		: SystemEventProcessor(std::move(dispatchCallback), std::move(contextCallback))
	{
	}

	void SystemEventProcessorSDL::ProcessEvents()
	{
		Uint32 windowId = UINT32_MAX;
		SDL_Event eventSDL;
		while (SDL_PollEvent(&eventSDL)) {
			if(eventSDL.window.windowID != windowId) {
				windowId = eventSDL.window.windowID;
			}

			SystemEvent::Context ctx = mContextCallback(SDLHelpers::GetPlatformWindowHandle(SDL_GetWindowFromID(windowId)));
			if(!ctx.mWindow || ctx.mWindow->GetWindowType() != WindowAPI::SDL) {
				continue;
			}

			switch (eventSDL.type) {
			case SDL_QUIT: {
			} break;
			case SDL_APP_TERMINATING: {
			} break;
			case SDL_APP_LOWMEMORY: {
			} break;
			case SDL_APP_WILLENTERBACKGROUND: {
			} break;
			case SDL_APP_DIDENTERBACKGROUND: {
			} break;
			case SDL_APP_WILLENTERFOREGROUND: {
			} break;
			case SDL_APP_DIDENTERFOREGROUND: {
			} break;
			case SDL_WINDOWEVENT: {
				switch (eventSDL.window.event) {
					case SDL_WINDOWEVENT_SHOWN: {
						mDispatchCallback({ SystemEvent::WindowVisibilityChanged{ false } }, ctx);
					} break;
					case SDL_WINDOWEVENT_HIDDEN: {
						mDispatchCallback({ SystemEvent::WindowVisibilityChanged{ true } }, ctx);
					} break;
					case SDL_WINDOWEVENT_EXPOSED: {
					} break;
					case SDL_WINDOWEVENT_MOVED: {
						mDispatchCallback({ SystemEvent::WindowMoved{ eventSDL.window.data1, eventSDL.window.data2 } }, ctx);
					} break;
						// This event is called after SDL_WINDOWEVENT_SIZE_CHANGED for external size change, user or window manager
					case SDL_WINDOWEVENT_RESIZED: {
					} break;
					case SDL_WINDOWEVENT_SIZE_CHANGED: {
						mDispatchCallback({ SystemEvent::WindowSizeChanged{ eventSDL.window.data1, eventSDL.window.data2 } }, ctx);
						mDispatchCallback({ SystemEvent::WindowResized{ eventSDL.window.data1, eventSDL.window.data2 } }, ctx);
					} break;
					case SDL_WINDOWEVENT_MINIMIZED: {
						mDispatchCallback({ SystemEvent::WindowMinimized{} }, ctx);
					} break;
					case SDL_WINDOWEVENT_MAXIMIZED: {
						mDispatchCallback({ SystemEvent::WindowMaximized{} }, ctx);
					} break;
					case SDL_WINDOWEVENT_RESTORED: {
						mDispatchCallback({ SystemEvent::WindowRestored{} }, ctx);
					} break;
					case SDL_WINDOWEVENT_ENTER: {
						mDispatchCallback({ SystemEvent::MouseEnter{} }, ctx);
					} break;
					case SDL_WINDOWEVENT_LEAVE: {
						mDispatchCallback({ SystemEvent::MouseExit{} }, ctx);
					} break;
					case SDL_WINDOWEVENT_FOCUS_GAINED: {
						mDispatchCallback({ SystemEvent::WindowFocused{} }, ctx);
					} break;
					case SDL_WINDOWEVENT_FOCUS_LOST: {
						mDispatchCallback({ SystemEvent::WindowUnfocused{} }, ctx);
					} break;
					case SDL_WINDOWEVENT_CLOSE: {
						mDispatchCallback({ SystemEvent::WindowClosed{} }, ctx);
					} break;
					case SDL_WINDOWEVENT_TAKE_FOCUS: {
					} break;
					case SDL_WINDOWEVENT_HIT_TEST: {
					} break;
					default: break;
				}
			} break;
			case SDL_KEYDOWN: {
				if (eventSDL.key.repeat == SDL_FALSE) {
					const KeyCode code = SDLHelpers::GetKeyCode(eventSDL.key.keysym.sym);
					mDispatchCallback({ SystemEvent::KeyDown{ code } }, ctx);
				}
			} break;
			case SDL_KEYUP: {
				if (eventSDL.key.repeat == SDL_FALSE) {
					const KeyCode code = SDLHelpers::GetKeyCode(eventSDL.key.keysym.sym);
					mDispatchCallback({ SystemEvent::KeyUp{ code } }, ctx);
				}
			} break;
			case SDL_TEXTINPUT: {
				mDispatchCallback({ SystemEvent::TextChar{ static_cast<unsigned char>(eventSDL.text.text[0]) } }, ctx);
			} break;
			case SDL_MOUSEBUTTONDOWN: {
				const MouseCode code = SDLHelpers::GetMouseCode(eventSDL.button.button);
				mDispatchCallback({ SystemEvent::MouseButtonDown{ code } }, ctx);
			} break;
			case SDL_MOUSEBUTTONUP: {
				const MouseCode code = SDLHelpers::GetMouseCode(eventSDL.button.button);
				mDispatchCallback({ SystemEvent::MouseButtonUp{ code } }, ctx);
			} break;
			case SDL_MOUSEMOTION: {
				mDispatchCallback({ SystemEvent::MouseMoved{ eventSDL.button.x, eventSDL.button.y } }, ctx);
			} break;
			case SDL_MOUSEWHEEL: {
				mDispatchCallback({ SystemEvent::MouseScroll{ static_cast<float>(eventSDL.wheel.x), static_cast<float>(eventSDL.wheel.y) } }, ctx);
			} break;
			default:;
			}
		}
	}
}

#endif