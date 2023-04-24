// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

#include <GUI/ImGui/ImGuiRenderer.h>
#include <imgui/imgui.h>
#include <Input/KeyCode.h>

struct ImGuiContext;

// TODO: Modify Cursor Type (e.g. drag arrow)
namespace Zeron
{
	namespace Gfx
	{
		class Graphics;
	}

	class SystemEvent;
	class Window;
	struct WindowEvent;

	class ImGuiInstance {
	  public:
		ImGuiInstance();
		~ImGuiInstance();

		bool Init(Gfx::Graphics& graphics, Gfx::GraphicsContext& graphicsContext);
		bool HandleEvent(const SystemEvent& evt) const;
		void NewFrame();
		void Update(Gfx::Graphics& graphics);
		void Draw(Gfx::CommandBuffer& cmd) const;

	  private:
		bool _onKey(KeyCode::Type code, bool isPressed) const;

		ImGuiContext* mContext;
		std::unique_ptr<ImGuiRenderer> mImGuiRenderer;

		// TODO: Change this with Timer wrapper later
		std::chrono::high_resolution_clock::time_point mTime;
	};
}