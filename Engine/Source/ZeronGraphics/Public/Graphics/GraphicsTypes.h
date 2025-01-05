// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

namespace Zeron::Gfx
{
	enum class GraphicsType
	{
		Null,
		Direct3D11,
		Vulkan
	};

	// ============  Pipeline Types  ============

	enum class PrimitiveTopology
	{
		Invalid = 0,
		PointList,
		LineList,
		TriangleList,
	};

	enum class FaceCullMode
	{
		None,
		Back,
		Front,
		FrontAndBack
	};

	enum class DepthMode
	{
		Default,
		Disabled,
		NoWrite,
	};

	enum class PipelineResourceType
	{
		Sampler,
		Texture,
		UniformBuffer,
		StorageBuffer,
		StorageTexture,
	};

	enum class BufferType
	{
		Undefined,
		Vertex,
		Index,
		Uniform,
		Storage,
	};

	enum class BufferUsageType
	{
		// GPU Only Access
		Default,

		// CPU Only Access
		Staging,

		// Shared Access
		Dynamic,

		Immutable,
	};

	enum class BufferUpdateRule
	{
		UnmapMemory,				// Unmap memory after update
		KeepMappedMemoryIfAllowed,	// Keep mapped memory only on supported APIs
		DoNothing,					// Keep existing memory state
	};

	enum class ShaderType
	{
		Vertex,
		Fragment,
		Compute,
	};

	enum class TextureFormat
	{
		Undefined,
		RGB_8U,
		RGBA_8U,
		BGRA_8U,
		Depth_16U,
		Depth_32F,
		DepthStencil_32U
	};

	enum class TextureType
	{
		Undefined,
		Sampling,
		RenderTarget,
		Depth,
	};

	enum class TextureLayout
	{
		Undefined,
		ColorAttachment,
		DepthStencilAttachment,
		ShaderReadOnly,
		TransferSrc,
		TransferDst,
		Present,
	};

	enum class MSAALevel
	{
		Disabled = 1,
		x2 = 2,
		x4 = 4,
		x8 = 8
	};

	enum class SamplerAddressMode
	{
		Repeat,
		Clamp
	};
}