// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

#include <Graphics/GraphicsTypes.h>

namespace Zeron::Gfx
{
	class Buffer;
	class Texture;
	class Sampler;

	class UniformBindingHandle {
	  public:
		Buffer* mBuffer;
	};

	class StorageBufferBindingHandle {
	  public:
		Buffer* mBuffer;
	};

	class SamplerBindingHandle {
	  public:
		Sampler* mSampler;
	};

	class TextureBindingHandle {
	  public:
		Texture* mTexture;
	};

	using BindingHandle = std::variant<UniformBindingHandle, SamplerBindingHandle, TextureBindingHandle, StorageBufferBindingHandle>;

	class PipelineBinding {
	  public:
		virtual ~PipelineBinding() = default;
	};
}