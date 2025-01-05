// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

#include <Sandbox/Samples/IWindowSample.h>

namespace Zeron
{
	namespace Render
	{
		class FontAtlas;
		class Mesh;
	}
	namespace Gfx
	{
		class CommandBuffer;
		class ShaderProgram;
		class Pipeline;
		class PipelineBinding;
		class Buffer;
		class Sampler;
		class Graphics;
	}
	class Window;
}

namespace Sandbox
{
	struct SampleContext;

	class TextRenderingSample : public IWindowSample {
	  public:
		static constexpr char NAME[] = "Text Rendering";

		TextRenderingSample(IWindowSampleContext& ctx);
		~TextRenderingSample();
		void Init() override;
		void Draw(Zeron::Gfx::CommandBuffer& cmd) override;

	  private:
		std::string mText;
		std::unique_ptr<Zeron::Gfx::Sampler> mSampler;
		std::shared_ptr<Zeron::Gfx::ShaderProgram> mShader;
		std::unique_ptr<Zeron::Gfx::Pipeline> mPipeline;
		std::unique_ptr<Zeron::Gfx::PipelineBinding> mFontBinding;
		std::unique_ptr<Zeron::Gfx::Buffer> mColorBuffer;

		std::unique_ptr<Zeron::Render::FontAtlas> mFont;
		std::unique_ptr<Zeron::Render::Mesh> mTextMesh;
	};
}