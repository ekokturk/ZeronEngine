// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

#include "Graphics/GraphicsTypes.h"

struct ImGuiContext;
struct ImVec2;

namespace Zeron
{
	class Buffer;
	class ShaderProgram;
	class PipelineBinding;
	class Pipeline;
	class Sampler;
	class Texture;
	class CommandBuffer;
	class Graphics;
	class GraphicsContext;

	class ImGuiRenderer final {
	  public:
		ImGuiRenderer();
		~ImGuiRenderer();

		bool Init(ImGuiContext& ctx, Graphics& graphics, GraphicsContext& graphicsContext);
		void NewFrame(ImGuiContext& ctx);
		void Update(ImGuiContext& ctx, Graphics& graphics);
		void Draw(ImGuiContext& ctx, CommandBuffer& cmd);
		void Destroy(ImGuiContext& ctx);

		ImVec2 GetDisplaySize() const;

	  private:
		struct ViewParams {
			Vec2 mScale;
			Vec2 mTranslation;
		};

		// TODO: This shouldn't be here maybe?
		Graphics* mGraphics;
		GraphicsContext* mGraphicsContext;

		std::unique_ptr<Buffer> mVertexBuffer;
		std::unique_ptr<Buffer> mIndexBuffer;
		std::unique_ptr<Buffer> mUniformBuffer;
		std::unique_ptr<Texture> mFontTexture;
		std::unique_ptr<Sampler> mSampler;
		std::unique_ptr<ShaderProgram> mShader;
		std::unique_ptr<Pipeline> mPipeline;
		std::unique_ptr<PipelineBinding> mBinding;
	};
}