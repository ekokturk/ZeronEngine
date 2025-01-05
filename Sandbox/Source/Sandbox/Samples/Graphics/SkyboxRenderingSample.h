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

	class SkyboxRenderingSample : public IWindowSample {
	  public:
		struct alignas(16) SkyboxSettings {
			Zeron::Vec4 mSunDirection = { 0.0f, -1.0f, 0.0f, 1.f };
			Zeron::Vec4 mSkyColor = { 0.53f, 0.81f, 0.92f, 1.f };
			Zeron::Vec4 mHorizonColor = { 1.0f, 0.8f, 0.6f, 1.f };
			Zeron::Vec4 mGroundColor = { 0.2f, 0.2f, 0.2f, 1.f };
			float mAtmosphereIntensity = 0.1f;
		};

		static constexpr char NAME[] = "Skybox";

		SkyboxRenderingSample(IWindowSampleContext& ctx);
		~SkyboxRenderingSample();

		void Init() override;
		void OnGui() override;
		void Draw(Zeron::Gfx::CommandBuffer& cmd) override;

	  private:
		std::shared_ptr<Zeron::Gfx::ShaderProgram> mShader;
		std::unique_ptr<Zeron::Gfx::Pipeline> mPipeline;
		std::unique_ptr<Zeron::Gfx::PipelineBinding> mPipelineBinding;
		std::unique_ptr<Zeron::Gfx::Buffer> mSettingsBuffer;
		std::unique_ptr<Zeron::Render::Mesh> mSkyboxMesh;

		SkyboxSettings mSkyboxSettings;
	};
}