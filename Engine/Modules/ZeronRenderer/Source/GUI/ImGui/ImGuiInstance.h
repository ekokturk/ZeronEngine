// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

#include <GUI/ImGui/ImGuiGraphics.h>
#include <imgui/imgui.h>
#include <Window/Input/KeyCode.h>

struct ImGuiContext;

// TODO: Modify Cursor Type (e.g. drag arrow)
namespace Zeron
{
	class Graphics;
	class Window;
	struct WindowEvent;

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


