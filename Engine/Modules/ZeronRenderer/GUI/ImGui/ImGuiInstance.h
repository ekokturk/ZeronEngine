// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

#include <GUI/ImGui/ImGuiRenderer.h>
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

		bool Init(Graphics& graphics, GraphicsContext& graphicsContext);
		bool HandleEvent(WindowEvent& evt) const;
		void NewFrame();
		void Update(Graphics& graphics);
		void Draw(CommandBuffer& cmd) const;

	private:
		bool _onKey(KeyCode::Type code, bool isPressed) const;

		ImGuiContext* mContext;
		std::unique_ptr<ImGuiRenderer> mImGuiRenderer;

		// TODO: Change this with Timer wrapper later
		std::chrono::high_resolution_clock::time_point mTime;
	};
}


