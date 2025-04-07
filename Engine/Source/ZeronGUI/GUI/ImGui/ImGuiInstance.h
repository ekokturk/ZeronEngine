// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

#include <Core/Input/KeyCode.h>
#include <GUI/ImGui/ImGuiRenderer.h>
#include <imgui/imgui.h>

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

		bool Init(Gfx::Graphics& graphics, Gfx::GraphicsContext& graphicsContext, std::shared_ptr<Gfx::ShaderProgram> shaderProgram);
		bool HandleEvent(const SystemEvent& evt) const;
		void NewFrame();
		void Update(Gfx::Graphics& graphics);
		void Draw(Gfx::CommandBuffer& cmd) const;

	  private:
		bool _onKey(KeyCode::Type code, bool isPressed) const;
		void _applyStyle();

		ImGuiContext* mContext;
		std::unique_ptr<ImGuiRenderer> mImGuiRenderer;

		// TODO: Change this with Timer wrapper later
		std::chrono::high_resolution_clock::time_point mTime;
	};
}