// Copyright (C) Eser Kokturk. All Rights Reserved.

#if ZE_PLATFORM_ANDROID

#	include <Platform/API/Android/PlatformAndroid.h>

#	include <Input/KeyCode.h>
#	include <Platform/API/Android/FileSystemHandlerAndroid.h>
#	include <Platform/API/Android/WindowAndroid.h>
#	include <Platform/SystemEvent.h>
#	include <Platform/WindowManager.h>

#	include <android/asset_manager.h>
#	include <android/asset_manager_jni.h>
#	include <android/input.h>
#	include <android/keycodes.h>
#	include <game-activity/GameActivity.h>
#	include <game-activity/GameActivityEvents.h>
#	include <game-activity/native_app_glue/android_native_app_glue.h>

extern "C" void android_main(struct android_app* app)
{
	ZE_LOGE("Dummy main");
}

namespace Zeron
{
	namespace
	{
		void HandleAppCmd(android_app* app, int cmd)
		{
			PlatformAndroid* platform = reinterpret_cast<PlatformAndroid*>(app->userData);
			if (platform) {
				platform->onAppCmd(cmd);
			}
		}
	}

	PlatformAndroid::PlatformAndroid(android_app* app)
		: Platform()
		, mApp(app)
		, mDestroyRequested(false)
	{}

	PlatformAndroid::~PlatformAndroid() {}

	bool PlatformAndroid::Init()
	{
		mApp->userData = this;
		mApp->onAppCmd = HandleAppCmd;
		// mApp->activity->callbacks->onContentRectChanged

		// Set event filters
		android_app_set_key_event_filter(mApp, NULL);
		android_app_set_motion_event_filter(mApp, NULL);

		// Poll until window is initialized
		while (!mWindow) {
			_processEvents();
			if (mDestroyRequested) {
				return false;
			}
		}

		mFileSystem = std::make_unique<FileSystemHandlerAndroid>(mApp->activity->assetManager);

		return true;
	}

	void PlatformAndroid::Update()
	{
		_processEvents();
	}

	void PlatformAndroid::_processEvents()
	{
		int ident = -1;
		int events = -1;
		android_poll_source* source = nullptr;
		while ((ident = ALooper_pollAll(0, nullptr, &events, reinterpret_cast<void**>(&source))) >= 0) {
			if (source) {
				source->process(mApp, source);
			}
			if (mApp->destroyRequested) {
				mDestroyRequested = true;
			}
		}
		_handleInputEvents();
	}

	void PlatformAndroid::onAppCmd(int cmd)
	{
		switch (static_cast<NativeAppGlueAppCmd>(cmd)) {
			// --- INIT
			case APP_CMD_START: {
				ZE_LOGI("Zeron Android application started (SDK {}).", mApp->activity->sdkVersion);
			} break;
			case APP_CMD_INIT_WINDOW: {
				if (!mWindow) {
					mWindow = std::make_unique<WindowAndroid>(WindowConfig{}, mApp->window);
				}
			} break;
			// --- LOOP
			case APP_CMD_WINDOW_RESIZED: {
				if (mWindow) {
					const int width = mApp->contentRect.right - mApp->contentRect.left;
					const int height = mApp->contentRect.bottom - mApp->contentRect.top;
					mWindow->OnSystemEvent({ SystemEvent::WindowResized{ width, height } });
				}
			} break;
			case APP_CMD_GAINED_FOCUS: {
				if (mWindow) {
					mWindow->OnSystemEvent({ SystemEvent::WindowFocused{} });
				}
			} break;
			case APP_CMD_LOST_FOCUS: {
				if (mWindow) {
					mWindow->OnSystemEvent({ SystemEvent::WindowUnfocused{} });
				}
			} break;
			// --- CLEANUP
			case APP_CMD_TERM_WINDOW: {
				// TODO: Window cleanup
			} break;
			case APP_CMD_DESTROY: {
				// TODO: Platform cleanup
				ZE_LOGI("Zeron Android application destroyed.");
			} break;
			default: break;
		}
	}

	void PlatformAndroid::_handleInputEvents()
	{
		android_input_buffer* inputBuffer = android_app_swap_input_buffers(mApp);
		if (!inputBuffer || !mWindow) {
			return;
		}

		// Touch / Mouse Events
		if (inputBuffer->motionEventsCount > 0) {
			for (int id = 0; id < inputBuffer->motionEventsCount; ++id) {
				GameActivityMotionEvent* event = &inputBuffer->motionEvents[id];
				const int source = event->source;
				const int action = event->action & AMOTION_EVENT_ACTION_MASK;
				const float x = GameActivityPointerAxes_getX(&event->pointers[0]);
				const float y = GameActivityPointerAxes_getY(&event->pointers[0]);

				if (source & AINPUT_SOURCE_MOUSE) {
					const int actionButton = event->actionButton;
					const int buttonState = event->buttonState;

					switch (action) {
						case AMOTION_EVENT_ACTION_DOWN:
						case AMOTION_EVENT_ACTION_POINTER_DOWN: {
							// ZE_LOG("Mouse Event Down: {} x{} y{}", action, x, y);
							mWindow->OnSystemEvent({ SystemEvent::MouseButtonDown{ MouseCode::LeftButton } });
						} break;
						case AMOTION_EVENT_ACTION_MOVE: {
							// TODO: Maybe use mouse delta
							// ZE_LOG("Mouse Event Move: {} x{} y{}", action, x, y);
							mWindow->OnSystemEvent({ SystemEvent::MouseMoved{ static_cast<int>(x), static_cast<int>(y) } });
						} break;
						case AMOTION_EVENT_ACTION_UP:
						case AMOTION_EVENT_ACTION_POINTER_UP: {
							// ZE_LOG("Mouse Event Up: {} x{} y{}", action, x, y);
							mWindow->OnSystemEvent({ SystemEvent::MouseButtonUp{ MouseCode::LeftButton } });
						} break;
						default: break;
					}
				}
				if (source & AINPUT_SOURCE_TOUCHSCREEN) {
					// ZE_LOG("Touch Event: {} x{} y{}", action, x, y);
					const size_t pointer_count = event->pointerCount;
					const int pointer_id = event->pointers[0].id;
					// TODO: Add touch event
				}
			}
			android_app_clear_motion_events(inputBuffer);
		}

		// Key Events
		if (inputBuffer->keyEventsCount > 0) {
			for (int id = 0; id < inputBuffer->keyEventsCount; ++id) {
				GameActivityKeyEvent* event = &inputBuffer->keyEvents[id];
				const int32_t action = event->action;
				const KeyCode keyCode = GetKeyCode(event->keyCode);

				if (action & AKEY_EVENT_ACTION_DOWN) {
					// ZE_LOG("KEY_DOWN {} {}", keyCode.ToString(), event->keyCode);
					mWindow->OnSystemEvent({ SystemEvent::KeyDown{ keyCode } });
				}
				else if (action & AKEY_EVENT_ACTION_UP) {
					// ZE_LOG("KEY_UP {} {}", keyCode.ToString(), event->keyCode);
					mWindow->OnSystemEvent({ SystemEvent::KeyDown{ keyCode } });
				}
			}
			android_app_clear_key_events(inputBuffer);
		}
	}

	Window* PlatformAndroid::GetMainWindow() const
	{
		return mWindow.get();
	}

	Window* PlatformAndroid::CreatePlatformWindow(const WindowConfig& config)
	{
		return mWindow.get();
	}

	void PlatformAndroid::_initDirectories() {}

	KeyCode PlatformAndroid::GetKeyCode(int keyCode)
	{
		switch (keyCode) {
			case AKEYCODE_0: return KeyCode::N0;
			case AKEYCODE_1: return KeyCode::N1;
			case AKEYCODE_2: return KeyCode::N2;
			case AKEYCODE_3: return KeyCode::N3;
			case AKEYCODE_4: return KeyCode::N4;
			case AKEYCODE_5: return KeyCode::N5;
			case AKEYCODE_6: return KeyCode::N6;
			case AKEYCODE_7: return KeyCode::N7;
			case AKEYCODE_8: return KeyCode::N8;
			case AKEYCODE_9: return KeyCode::N9;
			case AKEYCODE_A: return KeyCode::A;
			case AKEYCODE_B: return KeyCode::B;
			case AKEYCODE_C: return KeyCode::C;
			case AKEYCODE_D: return KeyCode::D;
			case AKEYCODE_E: return KeyCode::E;
			case AKEYCODE_F: return KeyCode::F;
			case AKEYCODE_G: return KeyCode::G;
			case AKEYCODE_H: return KeyCode::H;
			case AKEYCODE_I: return KeyCode::I;
			case AKEYCODE_J: return KeyCode::J;
			case AKEYCODE_K: return KeyCode::K;
			case AKEYCODE_L: return KeyCode::L;
			case AKEYCODE_M: return KeyCode::M;
			case AKEYCODE_N: return KeyCode::N;
			case AKEYCODE_O: return KeyCode::O;
			case AKEYCODE_P: return KeyCode::P;
			case AKEYCODE_Q: return KeyCode::Q;
			case AKEYCODE_R: return KeyCode::R;
			case AKEYCODE_S: return KeyCode::S;
			case AKEYCODE_T: return KeyCode::T;
			case AKEYCODE_U: return KeyCode::U;
			case AKEYCODE_V: return KeyCode::V;
			case AKEYCODE_W: return KeyCode::W;
			case AKEYCODE_X: return KeyCode::X;
			case AKEYCODE_Y: return KeyCode::Y;
			case AKEYCODE_Z: return KeyCode::Z;
			case AKEYCODE_ESCAPE: return KeyCode::Escape;
			case AKEYCODE_ENTER: return KeyCode::Enter;
			case AKEYCODE_TAB: return KeyCode::Tab;
			case AKEYCODE_BACK: return KeyCode::Backspace;
			case AKEYCODE_INSERT: return KeyCode::Insert;
			case AKEYCODE_DEL: return KeyCode::Delete;
			case AKEYCODE_SHIFT_LEFT: return KeyCode::LeftShift;
			case AKEYCODE_SHIFT_RIGHT: return KeyCode::RightShift;
			case AKEYCODE_CTRL_LEFT: return KeyCode::LeftControl;
			case AKEYCODE_CTRL_RIGHT: return KeyCode::RightControl;
			case AKEYCODE_ALT_LEFT: return KeyCode::LeftAlt;
			case AKEYCODE_ALT_RIGHT: return KeyCode::RightAlt;
			// case :						return KeyCode::LeftSuper;
			// case :						return KeyCode::RightSuper;
			case AKEYCODE_DPAD_RIGHT: return KeyCode::Right;
			case AKEYCODE_DPAD_LEFT: return KeyCode::Left;
			case AKEYCODE_DPAD_DOWN: return KeyCode::Down;
			case AKEYCODE_DPAD_UP: return KeyCode::Up;
			case AKEYCODE_PAGE_UP: return KeyCode::PageUp;
			case AKEYCODE_PAGE_DOWN: return KeyCode::PageDown;
			case AKEYCODE_HOME: return KeyCode::Home;
			case AKEYCODE_MOVE_END: return KeyCode::End;
			case AKEYCODE_CAPS_LOCK: return KeyCode::CapsLock;
			case AKEYCODE_SCROLL_LOCK: return KeyCode::ScrollLock;
			case AKEYCODE_NUM_LOCK: return KeyCode::NumLock;
			case AKEYCODE_SYSRQ: return KeyCode::PrintScreen;
			case AKEYCODE_BREAK: return KeyCode::Pause;
			case AKEYCODE_F1: return KeyCode::F1;
			case AKEYCODE_F2: return KeyCode::F2;
			case AKEYCODE_F3: return KeyCode::F3;
			case AKEYCODE_F4: return KeyCode::F4;
			case AKEYCODE_F5: return KeyCode::F5;
			case AKEYCODE_F6: return KeyCode::F6;
			case AKEYCODE_F7: return KeyCode::F7;
			case AKEYCODE_F8: return KeyCode::F8;
			case AKEYCODE_F9: return KeyCode::F9;
			case AKEYCODE_F10: return KeyCode::F10;
			case AKEYCODE_F11: return KeyCode::F11;
			case AKEYCODE_F12: return KeyCode::F12;
			// case :						return KeyCode::F13;
			// case :						return KeyCode::F14;
			// case :						return KeyCode::F15;
			// case :						return KeyCode::F16;
			// case :						return KeyCode::F17;
			// case :						return KeyCode::F18;
			// case :						return KeyCode::F19;
			// case :						return KeyCode::F20;
			// case :						return KeyCode::F21;
			// case :						return KeyCode::F22;
			// case :						return KeyCode::F23;
			// case :						return KeyCode::F24;
			case AKEYCODE_NUMPAD_0: return KeyCode::KP0;
			case AKEYCODE_NUMPAD_1: return KeyCode::KP1;
			case AKEYCODE_NUMPAD_2: return KeyCode::KP2;
			case AKEYCODE_NUMPAD_3: return KeyCode::KP3;
			case AKEYCODE_NUMPAD_4: return KeyCode::KP4;
			case AKEYCODE_NUMPAD_5: return KeyCode::KP5;
			case AKEYCODE_NUMPAD_6: return KeyCode::KP6;
			case AKEYCODE_NUMPAD_7: return KeyCode::KP7;
			case AKEYCODE_NUMPAD_8: return KeyCode::KP8;
			case AKEYCODE_NUMPAD_9: return KeyCode::KP9;
			case AKEYCODE_NUMPAD_DOT: return KeyCode::KPDecimal;
			case AKEYCODE_NUMPAD_DIVIDE: return KeyCode::KPDivide;
			case AKEYCODE_NUMPAD_MULTIPLY: return KeyCode::KPMultiply;
			case AKEYCODE_NUMPAD_SUBTRACT: return KeyCode::KPSubtract;
			case AKEYCODE_NUMPAD_ADD: return KeyCode::KPAdd;
			case AKEYCODE_NUMPAD_ENTER: return KeyCode::KPEnter;
			case AKEYCODE_NUMPAD_EQUALS: return KeyCode::KPEqual;
			case AKEYCODE_SPACE: return KeyCode::Space;
			case AKEYCODE_APOSTROPHE: return KeyCode::Apostrophe;
			case AKEYCODE_COMMA: return KeyCode::Comma;
			case AKEYCODE_MINUS: return KeyCode::Minus;
			case AKEYCODE_PERIOD: return KeyCode::Period;
			case AKEYCODE_SLASH: return KeyCode::Slash;
			case AKEYCODE_SEMICOLON: return KeyCode::Semicolon;
			case AKEYCODE_PLUS: return KeyCode::Equal;
			case AKEYCODE_LEFT_BRACKET: return KeyCode::LeftBracket;
			case AKEYCODE_RIGHT_BRACKET: return KeyCode::RightBracket;
			case AKEYCODE_BACKSLASH:
				return KeyCode::Backslash;
				// case :						return KeyCode::Tilde;
				// case :						return KeyCode::Menu;
		}
		return KeyCode::Unknown;
	}
}

#endif