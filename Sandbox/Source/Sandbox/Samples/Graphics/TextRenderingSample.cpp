// Copyright (C) Eser Kokturk. All Rights Reserved.

#include <Core/FileSystem.h>
#include <Sandbox/Samples/Graphics/TextRenderingSample.h>

#include <Render/Camera/Camera.h>
#include <Render/Mesh/Model.h>

#include <Graphics/Buffer.h>
#include <Graphics/CommandBuffer.h>
#include <Graphics/Graphics.h>
#include <Graphics/GraphicsContext.h>
#include <Graphics/Pipeline.h>
#include <Graphics/PipelineBinding.h>
#include <Graphics/Sampler.h>
#include <Graphics/ShaderProgram.h>
#include <Graphics/ShaderProgramConfig.h>
#include <Render/Font/FontAtlas.h>
#include <Render/Mesh/MeshData.h>


using namespace Zeron;

namespace Sandbox
{
	TextRenderingSample::TextRenderingSample(IWindowSampleContext& ctx)
		: IWindowSample(ctx)
		, mText("Test String!")
	{
		mHasGui = false;
	}

	TextRenderingSample::~TextRenderingSample() = default;

	void TextRenderingSample::Init()
	{
		auto font = Locator::Get<FileSystem>()->ReadBinaryFile(Path("Resources/Fonts/NimbusSans.ttf"));
		ZE_ASSERT(!font.HasError(), "");

		mFont = std::make_unique<Render::FontAtlas>("Arial");
		mFont->Load(mCtx.GetGraphics(), font.Value());

		mShader = mCtx.LoadShader("Basic2D");

		mSampler = mCtx.GetGraphics().CreateSampler();
		mColorBuffer = mCtx.GetGraphics().CreateUniformBuffer(Color(Color::Palette::White).norm());

		mPipeline = mCtx.GetGraphics().CreatePipelineGraphics(
			mShader.get(),
			mCtx.GetGraphicsContext().GetSwapChainRenderPass(),
			{ mCtx.GetGraphics().GetMultiSamplingLevel(), Gfx::PrimitiveTopology::TriangleList, true, Gfx::FaceCullMode::None }
		);
		mFontBinding = mCtx.GetGraphics().CreatePipelineBinding(
			*mPipeline,
			{ {
				Gfx::TextureBindingHandle{ mFont->GetTexture() },
				Gfx::SamplerBindingHandle{ mSampler.get() },
				Gfx::UniformBindingHandle{ mColorBuffer.get() },
			} }
		);

		mTextMesh = std::make_unique<Render::Mesh>(mCtx.GetGraphics(), mFont->GenerateText(mText));
	}

	void TextRenderingSample::Draw(Gfx::CommandBuffer& cmd)
	{
		cmd.BeginDebugGroup("Text Sample - Render");
		cmd.SetPipeline(*mPipeline);
		cmd.SetPipelineBinding(*mFontBinding);
		constexpr std::array attr = { Render::MeshAttribute::ScreenPos, Render::MeshAttribute::UV };
		mTextMesh->Bind(cmd, attr);

		{
			cmd.BeginDebugGroup("Draw World Text");
			Mat4 ubo = { mCtx.GetCamera().GetProjectionMatrix() * mCtx.GetCamera().GetViewMatrix() * glm::translate(glm::mat4(1.0f), { -1.f, 2.5f, 0.f }) };
			ubo = Math::Rotate(ubo, Math::PI<float>(), Math::RIGHT_DIR<float>);
			ubo = Math::Scale(ubo, { 1.f / 100 });
			cmd.SetPushConstant<Mat4>(&ubo, Gfx::ShaderType::Vertex);
			cmd.DrawIndexed(mTextMesh->GetIndexCount());
			cmd.EndDebugGroup();
		}

		{
			cmd.BeginDebugGroup("Draw Screen Text");
			const Mat4 proj = Math::Orthographic(0.f, 800.f, 600.f, 0.f, .0f, 1.f);
			cmd.SetPushConstant<Mat4>(&proj, Gfx::ShaderType::Vertex);
			cmd.DrawIndexed(mTextMesh->GetIndexCount());
			cmd.EndDebugGroup();
		}

		cmd.EndDebugGroup();
	}


}