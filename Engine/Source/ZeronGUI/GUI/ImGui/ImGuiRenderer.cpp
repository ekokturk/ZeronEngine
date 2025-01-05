// Copyright (C) Eser Kokturk. All Rights Reserved.

#include <GUI/ImGui/ImGuiRenderer.h>

#include <Graphics/Buffer.h>
#include <Graphics/CommandBuffer.h>
#include <Graphics/Graphics.h>
#include <Graphics/GraphicsContext.h>
#include <Graphics/Pipeline.h>
#include <Graphics/PipelineBinding.h>
#include <Graphics/Sampler.h>
#include <Graphics/ShaderProgram.h>
#include <Graphics/Texture.h>
#include <Graphics/VertexLayout.h>
#include <imgui/imgui.h>
#include <Platform/FileSystem.h>

namespace Zeron
{
	ImGuiRenderer::ImGuiRenderer(ImGuiContext& ctx, Gfx::Graphics& graphics, Gfx::GraphicsContext& graphicsContext, std::shared_ptr<Gfx::ShaderProgram> shaderProgram)
		: mGraphics(&graphics)
		, mGraphicsContext(&graphicsContext)
	{
		ImGui::SetCurrentContext(&ctx);

		// Initialize Font Texture
		ImGuiIO& io = ImGui::GetIO();
		unsigned char* fontData = nullptr;
		int texWidth = 0, texHeight = 0;
		io.Fonts->GetTexDataAsRGBA32(&fontData, &texWidth, &texHeight);
		mFontTexture = graphics.CreateTexture({ texWidth, texHeight }, Gfx::TextureFormat::RGBA_8U, fontData);
		mSampler = graphics.CreateSampler(Gfx::SamplerAddressMode::Clamp, false);
		mShader = shaderProgram;
		// TODO: We should use MSAA::Disabled for this
		mPipeline = graphics.CreatePipelineGraphics(
			mShader.get(), graphicsContext.GetSwapChainRenderPass(), { graphics.GetMultiSamplingLevel(), Gfx::PrimitiveTopology::TriangleList, true, Gfx::FaceCullMode::None }
		);
		mBinding = graphics.CreatePipelineBinding(
			*mPipeline,
			{ {
				Gfx::TextureBindingHandle{ mFontTexture.get() },
				Gfx::SamplerBindingHandle{ mSampler.get() },
			} }
		);
	}

	ImGuiRenderer::~ImGuiRenderer() {}

	void ImGuiRenderer::NewFrame(ImGuiContext& ctx)
	{
		ImGui::SetCurrentContext(&ctx);
	}

	void ImGuiRenderer::Update(ImGuiContext& ctx, Gfx::Graphics& graphics)
	{
		ImGui::SetCurrentContext(&ctx);
		ImDrawData* imDrawData = ImGui::GetDrawData();

		// ---- Prepare Resources

		const size_t vertexBufferByteSize = imDrawData->TotalVtxCount * sizeof(ImDrawVert);
		const size_t indexBufferByteSize = imDrawData->TotalIdxCount * sizeof(ImDrawIdx);
		if (vertexBufferByteSize == 0 || indexBufferByteSize == 0) {
			return;
		}

		if (!mVertexBuffer || mVertexBuffer->GetCount() != static_cast<uint32_t>(imDrawData->TotalVtxCount)) {
			mVertexBuffer = mGraphics->CreateBuffer(Gfx::BufferType::Vertex, imDrawData->TotalVtxCount, sizeof(ImDrawVert), nullptr, Gfx::BufferUsageType::Dynamic);
			mVertexBuffer->SetDebugName("ImGui - Vertex Buffer");
		}
		if (!mIndexBuffer || mIndexBuffer->GetCount() < static_cast<uint32_t>(imDrawData->TotalIdxCount)) {
			mIndexBuffer = mGraphics->CreateBuffer(Gfx::BufferType::Index, imDrawData->TotalIdxCount, sizeof(ImDrawIdx), nullptr, Gfx::BufferUsageType::Dynamic);
			mIndexBuffer->SetDebugName("ImGui - Index Buffer");
		}
	}

	void ImGuiRenderer::Draw(ImGuiContext& ctx, Gfx::CommandBuffer& cmd)
	{
		if (!mVertexBuffer || !mIndexBuffer) {
			return;
		}

		if (mVertexBuffer->GetCount() == 0) {
			return;
		}

		ImGui::SetCurrentContext(&ctx);
		ImGuiIO& io = ImGui::GetIO();
		ImDrawData* imDrawData = ImGui::GetDrawData();


		cmd.BeginDebugGroup("Prepare Pipeline");
		{
			cmd.BeginDebugGroup("Update Buffers");
			int offsetVertex = 0;
			int offsetIndex = 0;
			for (int i = 0; i < imDrawData->CmdListsCount; ++i) {
				const ImDrawList* cmdList = imDrawData->CmdLists[i];
				const Gfx::BufferUpdateRule updateRule = i + 1 == imDrawData->CmdListsCount ? Gfx::BufferUpdateRule::KeepMappedMemoryIfAllowed :
																							  Gfx::BufferUpdateRule::DoNothing;
				cmd.UpdateBuffer(*mVertexBuffer, &cmdList->VtxBuffer.Data[0], cmdList->VtxBuffer.Size * sizeof(ImDrawVert), offsetVertex, updateRule);
				cmd.UpdateBuffer(*mIndexBuffer, &cmdList->IdxBuffer.Data[0], cmdList->IdxBuffer.Size * sizeof(ImDrawIdx), offsetIndex, updateRule);
				offsetVertex += cmdList->VtxBuffer.Size;
				offsetIndex += cmdList->IdxBuffer.Size;
			}
			cmd.EndDebugGroup();

			if (imDrawData->CmdListsCount > 0) {
				cmd.SetPipeline(*mPipeline);
				cmd.SetPipelineBinding(*mBinding);
				cmd.SetVertexBuffer(*mVertexBuffer);
				cmd.SetIndexBuffer(*mIndexBuffer);

				Mat4 proj = Math::Orthographic(
					imDrawData->DisplayPos.x,
					imDrawData->DisplayPos.x + imDrawData->DisplaySize.x,
					imDrawData->DisplayPos.y + imDrawData->DisplaySize.y,
					imDrawData->DisplayPos.y,
					.0f,
					1.f
				);
				cmd.SetPushConstant<Mat4>(&proj, Gfx::ShaderType::Vertex);
			}
		}
		cmd.EndDebugGroup();

		uint32_t vertexOffset = 0;
		uint32_t indexOffset = 0;
		for (int32_t i = 0; i < imDrawData->CmdListsCount; ++i) {
			ImVec2 clipOff = imDrawData->DisplayPos;
			const ImDrawList* cmdList = imDrawData->CmdLists[i];
			for (int32_t j = 0; j < cmdList->CmdBuffer.Size; ++j) {
				cmd.BeginDebugGroup("Draw GUI Command");
				const ImDrawCmd* imCmd = &cmdList->CmdBuffer[j];
				cmd.SetScissor(
					{ static_cast<int>(imCmd->ClipRect.z - clipOff.x), static_cast<int>(imCmd->ClipRect.w - clipOff.y) },
					{ static_cast<int>(imCmd->ClipRect.x - clipOff.x), static_cast<int>(imCmd->ClipRect.y - clipOff.y) }
				);
				cmd.DrawIndexed(imCmd->ElemCount, indexOffset, vertexOffset);
				indexOffset += imCmd->ElemCount;
				cmd.EndDebugGroup();
			}
			vertexOffset += cmdList->VtxBuffer.Size;
		}
	}

	void ImGuiRenderer::Destroy(ImGuiContext& ctx) {}

	ImVec2 ImGuiRenderer::GetDisplaySize() const
	{
		const Vec2i size = mGraphicsContext ? mGraphicsContext->GetSwapChainSize() : Vec2i();
		return { static_cast<float>(size.X), static_cast<float>(size.Y) };
	}
}