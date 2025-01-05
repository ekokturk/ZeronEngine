// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

namespace Zeron::Render
{
	enum class MeshAttribute
	{
		ScreenPos = 1 << 0,
		WorldPos = 1 << 1,
		UV = 1 << 2,
		Normal = 1 << 3,
		Tangent = 1 << 4,
		Color = 1 << 5,
		BoneIndices = 1 << 6,
		BoneWeights = 1 << 7,
	};


	template <MeshAttribute E>
	struct MeshAttributeInfo {};

	template <>
	struct MeshAttributeInfo<MeshAttribute::ScreenPos> {
		using Type = Vec2;
	};

	template <>
	struct MeshAttributeInfo<MeshAttribute::WorldPos> {
		using Type = Vec3;
	};

	template <>
	struct MeshAttributeInfo<MeshAttribute::UV> {
		using Type = Vec2;
	};

	template <>
	struct MeshAttributeInfo<MeshAttribute::Normal> {
		using Type = Vec3;
	};

	template <>
	struct MeshAttributeInfo<MeshAttribute::Tangent> {
		using Type = Vec4;
	};


	inline size_t GetMeshAttributeSize(MeshAttribute attribute)
	{
		switch (attribute) {
			case MeshAttribute::WorldPos: return sizeof(typename MeshAttributeInfo<MeshAttribute::WorldPos>::Type);
			case MeshAttribute::Normal: return sizeof(typename MeshAttributeInfo<MeshAttribute::Normal>::Type);
			case MeshAttribute::UV: return sizeof(typename MeshAttributeInfo<MeshAttribute::UV>::Type);
			case MeshAttribute::Tangent: return sizeof(typename MeshAttributeInfo<MeshAttribute::Tangent>::Type);
			case MeshAttribute::ScreenPos: return sizeof(typename MeshAttributeInfo<MeshAttribute::ScreenPos>::Type);
			case MeshAttribute::Color:
			case MeshAttribute::BoneIndices:
			case MeshAttribute::BoneWeights:
			default: {
				ZE_FAIL("Unsupported Mesh atttirbute");
				return 0;
			}
		}
	}

}
