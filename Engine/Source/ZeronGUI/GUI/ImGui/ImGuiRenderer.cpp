// Copyright (C) Eser Kokturk. All Rights Reserved.

#include <GUI/ImGui/ImGuiRenderer.h>

#include <Graphics/Buffer.h>
#include <Graphics/CommandBuffer.h>
#include <Graphics/Graphics.h>
#include <Graphics/GraphicsContext.h>
#include <Graphics/Pipeline.h>
#include <Graphics/PipelineBinding.h>
#include <Graphics/Sampler.h>
#include <Graphics/Shader.h>
#include <Graphics/Texture.h>
#include <imgui/imgui.h>
#include <Platform/FileSystem.h>

namespace Zeron
{
	ImGuiRenderer::ImGuiRenderer()
		: mGraphics(nullptr)
	{}

	ImGuiRenderer::~ImGuiRenderer() {}

	bool ImGuiRenderer::Init(ImGuiContext& ctx, Graphics& graphics, GraphicsContext& graphicsContext)
	{
		mGraphics = &graphics;
		mGraphicsContext = &graphicsContext;

		ImGui::SetCurrentContext(&ctx);

		// Initialize Font Texture
		ImGuiIO& io = ImGui::GetIO();
		unsigned char* fontData = nullptr;
		int texWidth = 0, texHeight = 0;
		io.Fonts->GetTexDataAsRGBA32(&fontData, &texWidth, &texHeight);
		mFontTexture = graphics.CreateTexture(TextureType::Undefined, reinterpret_cast<Color*>(fontData), texWidth, texHeight);

		mSampler = graphics.CreateSampler(SamplerAddressMode::Clamp, false);
		mUniformBuffer = graphics.CreateUniformBuffer(Mat4{});
		// TODO: Don't use filesystem here
		auto vertexShaderBuffer = FileSystem::ReadBinaryFile(Path("Resources/Shaders") / graphics.GetCompiledShaderName("ImGui", ShaderType::Vertex));
		auto fragmentShaderBuffer = FileSystem::ReadBinaryFile(Path("Resources/Shaders") / graphics.GetCompiledShaderName("ImGui", ShaderType::Fragment));
		mShader = graphics.CreateShaderProgram(
			"ImGui",
			{
				{ "POSITION", VertexFormat::Float2 },
				{ "TEXCOORD", VertexFormat::Float2 },
				{ "COLOR", VertexFormat::Color },
			},
			{ {
				{ PipelineResourceType::UniformBuffer, ShaderType::Vertex, 0 },
				{ PipelineResourceType::Texture, ShaderType::Fragment, 1 },
				{ PipelineResourceType::Sampler, ShaderType::Fragment, 2 },
			} },
			vertexShaderBuffer.Value(),
			fragmentShaderBuffer.Value()
		);
		// TODO: We should use MSAA::Disabled for this
		mPipeline = graphics.CreatePipeline(
			mShader.get(), graphicsContext.GetSwapChainRenderPass(), graphics.GetMultiSamplingLevel(), PrimitiveTopology::TriangleList, true, FaceCullMode::None
		);
		mBinding = graphics.CreatePipelineBinding(
			*mPipeline,
			{ {
				UniformBindingHandle{ mUniformBuffer.get() },
				TextureBindingHandle{ mFontTexture.get() },
				SamplerBindingHandle{ mSampler.get() },
			} }
		);

		return true;
	}

	void ImGuiRenderer::NewFrame(ImGuiContext& ctx)
	{
		ImGui::SetCurrentContext(&ctx);
	}

	void ImGuiRenderer::Update(ImGuiContext& ctx, Graphics& graphics)
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
			mVertexBuffer = mGraphics->CreateBuffer(BufferType::Vertex, imDrawData->TotalVtxCount, sizeof(ImDrawVert), nullptr, BufferUsageType::Dynamic);
		}
		if (!mIndexBuffer || mIndexBuffer->GetCount() < static_cast<uint32_t>(imDrawData->TotalIdxCount)) {
			mIndexBuffer = mGraphics->CreateBuffer(BufferType::Index, imDrawData->TotalIdxCount, sizeof(ImDrawIdx), nullptr, BufferUsageType::Dynamic);
		}
	}

	void ImGuiRenderer::Draw(ImGuiContext& ctx, CommandBuffer& cmd)
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

		// ---- Draw
		Mat4 proj = Math::Orthographic(
			imDrawData->DisplayPos.x,
			imDrawData->DisplayPos.x + imDrawData->DisplaySize.x,
			imDrawData->DisplayPos.y + imDrawData->DisplaySize.y,
			imDrawData->DisplayPos.y,
			.0f,
			1.f
		);
		cmd.UpdateBuffer(*mUniformBuffer, &proj, sizeof(proj));

		cmd.SetPipeline(*mPipeline);
		cmd.SetPipelineBinding(*mBinding);

		int offsetVertex = 0;
		int offsetIndex = 0;
		for (int i = 0; i < imDrawData->CmdListsCount; ++i) {
			const ImDrawList* cmdList = imDrawData->CmdLists[i];
			const BufferUpdateRule updateRule = i + 1 == imDrawData->CmdListsCount ? BufferUpdateRule::KeepMappedMemoryIfAllowed : BufferUpdateRule::DoNothing;
			cmd.UpdateBuffer(*mVertexBuffer, &cmdList->VtxBuffer.Data[0], cmdList->VtxBuffer.Size * sizeof(ImDrawVert), offsetVertex, updateRule);
			cmd.UpdateBuffer(*mIndexBuffer, &cmdList->IdxBuffer.Data[0], cmdList->IdxBuffer.Size * sizeof(ImDrawIdx), offsetIndex, updateRule);
			offsetVertex += cmdList->VtxBuffer.Size;
			offsetIndex += cmdList->IdxBuffer.Size;
		}

		if (imDrawData->CmdListsCount > 0) {
			cmd.SetVertexBuffer(*mVertexBuffer);
			cmd.SetIndexBuffer(*mIndexBuffer);

			uint32_t vertexOffset = 0;
			uint32_t indexOffset = 0;
			for (int32_t i = 0; i < imDrawData->CmdListsCount; ++i) {
				ImVec2 clipOff = imDrawData->DisplayPos;

				const ImDrawList* cmdList = imDrawData->CmdLists[i];
				for (int32_t j = 0; j < cmdList->CmdBuffer.Size; ++j) {
					const ImDrawCmd* imCmd = &cmdList->CmdBuffer[j];
					cmd.SetScissor(
						{ static_cast<int>(imCmd->ClipRect.z - clipOff.x), static_cast<int>(imCmd->ClipRect.w - clipOff.y) },
						{ static_cast<int>(imCmd->ClipRect.x - clipOff.x), static_cast<int>(imCmd->ClipRect.y - clipOff.y) }
					);
					cmd.DrawIndexed(imCmd->ElemCount, indexOffset, vertexOffset);
					indexOffset += imCmd->ElemCount;
				}
				vertexOffset += cmdList->VtxBuffer.Size;
			}
		}
	}

	void ImGuiRenderer::Destroy(ImGuiContext& ctx) {}

	ImVec2 ImGuiRenderer::GetDisplaySize() const
	{
		const Vec2i size = mGraphicsContext ? mGraphicsContext->GetSwapChainSize() : Vec2i();
		return { static_cast<float>(size.X), static_cast<float>(size.Y) };
	}
}