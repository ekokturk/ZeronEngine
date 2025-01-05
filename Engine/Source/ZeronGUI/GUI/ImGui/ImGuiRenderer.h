// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

#include "Graphics/GraphicsTypes.h"

struct ImGuiContext;
struct ImVec2;

namespace Zeron
{
	namespace Gfx
	{
		struct ShaderProgramConfig;
		class Buffer;
		class ShaderProgram;
		class PipelineBinding;
		class Pipeline;
		class Sampler;
		class Texture;
		class CommandBuffer;
		class Graphics;
		class GraphicsContext;
	}

	class ImGuiRenderer final {
	  public:
		ImGuiRenderer(ImGuiContext& ctx, Gfx::Graphics& graphics, Gfx::GraphicsContext& graphicsContext, std::shared_ptr<Gfx::ShaderProgram>);
		~ImGuiRenderer();

		void NewFrame(ImGuiContext& ctx);
		void Update(ImGuiContext& ctx, Gfx::Graphics& graphics);
		void Draw(ImGuiContext& ctx, Gfx::CommandBuffer& cmd);
		void Destroy(ImGuiContext& ctx);

		ImVec2 GetDisplaySize() const;

	  private:
		struct ViewParams {
			Vec2 mScale;
			Vec2 mTranslation;
		};

		// TODO: This shouldn't be here maybe?
		Gfx::Graphics* mGraphics;
		Gfx::GraphicsContext* mGraphicsContext;

		std::unique_ptr<Gfx::Buffer> mVertexBuffer;
		std::unique_ptr<Gfx::Buffer> mIndexBuffer;
		std::unique_ptr<Gfx::Texture> mFontTexture;
		std::unique_ptr<Gfx::Sampler> mSampler;
		std::shared_ptr<Gfx::ShaderProgram> mShader;
		std::unique_ptr<Gfx::Pipeline> mPipeline;
		std::unique_ptr<Gfx::PipelineBinding> mBinding;
	};
}