// Copyright (C) 2020, Eser Kokturk. All Rights Reserved.

#include "ImGuiGraphics.h"
#include "Graphics/Graphics.h"
#if ZE_GRAPHICS_D3D
#include "imgui/imgui_impl_dx11.h"
#include "Graphics/API/D3D/11/GraphicsD3D11.h"
#endif

namespace Zeron
{
	ImGuiGraphics::ImGuiGraphics()
		: mGraphicsType(GraphicsType::Null)
	{
	}

	bool ImGuiGraphics::Init(ImGuiContext& ctx, Graphics& graphics)
	{
		bool isInit = false;
		mGraphicsType = graphics.GetGraphicsType();
		ZE_ASSERT(mGraphicsType != GraphicsType::Null, "Cannot have Null graphics!");

		ImGui::SetCurrentContext(&ctx);
		switch (mGraphicsType) {
			case GraphicsType::Direct3D11: {
			#if ZE_GRAPHICS_D3D
				auto& graphicsD3D11 = static_cast<GraphicsD3D11&>(graphics);
				isInit = ImGui_ImplDX11_Init(graphicsD3D11.GetDeviceD3D(),
					graphicsD3D11.GetDeviceContextD3D());
			#endif
			} break;
			default:;
		}
		return isInit;
	}

	void ImGuiGraphics::NewFrame(ImGuiContext& ctx) const
	{
		ImGui::SetCurrentContext(&ctx);
		switch (mGraphicsType) {
			case GraphicsType::Direct3D11: {
			#if ZE_GRAPHICS_D3D
				ImGui_ImplDX11_NewFrame();
			#endif
			} break;
			default:;
		}
	}

	void ImGuiGraphics::Draw(ImGuiContext& ctx) const
	{
		ImGui::SetCurrentContext(&ctx);
		switch (mGraphicsType) {
			case GraphicsType::Direct3D11: {
			#if ZE_GRAPHICS_D3D
				ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
			#endif
			} break;
			default:;
		}
	}

	void ImGuiGraphics::Destroy(ImGuiContext& ctx) const
	{
		ImGui::SetCurrentContext(&ctx);
		switch (mGraphicsType) {
			case GraphicsType::Direct3D11: {
				#if ZE_GRAPHICS_D3D
				ImGui_ImplDX11_Shutdown();
				#endif
			} break;
			default:;
		}
	}
}
