// Copyright (C) 2020, Eser Kokturk. All Rights Reserved.

#pragma once
#include "Graphics/GraphicsTypes.h"

struct ImGuiContext;

namespace Zeron
{
	class Graphics;

	// Wrapper class for ImGui graphics API implementations
	class ImGuiGraphics {
	public:
		ImGuiGraphics();
		
		bool Init(ImGuiContext& ctx, Graphics& graphics);
		void NewFrame(ImGuiContext& ctx) const;
		void Draw(ImGuiContext& ctx) const;
		void Destroy(ImGuiContext& ctx) const;

	private:
		GraphicsType mGraphicsType;
	};
}


