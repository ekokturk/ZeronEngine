// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

namespace Zeron
{
	enum class GraphicsType {
		Null,
		Direct3D11,
		Vulkan
	};

	// ============  Pipeline Types  ============

	enum class PrimitiveTopology
	{
		Invalid = 0,
		PointList,
		TriangleList,
	};

	enum class FaceCullMode {
		None,
		Back,
		Front,
		FrontAndBack
	};

	enum class PipelineResourceType {
		UniformBuffer,
		DynamicUniformBuffer,
		Sampler,
		Texture
	};
	
	enum class BufferType {
		Undefined,
		Vertex,
		Index,
		Uniform,
	};

	enum class BufferUsageType {
		Default,
		Staging, // Only CPU Access
		Dynamic, // Shared Access
		Immutable,
	};

	enum class BufferUpdateRule {
		UnmapMemory, // Unmap memory after update
		KeepMappedMemoryIfAllowed, // Keep mapped memory only on supported APIs
		DoNothing, // Keep existing memory state
	};

	enum class ShaderType {
		Vertex,
		Fragment,
		Compute,
	};

	enum class TextureType {
		Undefined,
		Invalid,
		Diffuse,
		Normal,
	};

	enum class MSAALevel {
		Disabled,
		x2,
		x4,
		x8
	};

	enum class SamplerAddressMode {
		Repeat,
		Clamp
	};
}
