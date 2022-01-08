// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

#include "imgui/imgui.h"
#include "ImGuiGraphics.h"
#include "Window/Input/KeyCode.h"

// TODO: Modify Cursor Type (e.g. drag arrow)

namespace Zeron
{
	struct WindowEvent;
	class Graphics;
	class Window;

	class ImGuiInstance {
	public:
		ImGuiInstance();
		~ImGuiInstance();

		bool Init(Graphics& graphics, Window& window);
		bool HandleEvent(WindowEvent& evt) const;
		void NewFrame();
		void Draw() const;

	private:
		bool OnKey_(KeyCode::Type code, bool isPressed) const;

		ImGuiContext* mContext;
		ImGuiGraphics mImGuiGraphics;
		Window* mWindow;

		// TODO: Change this with Timer wrapper later
		std::chrono::high_resolution_clock::time_point mTime;
	};
}


