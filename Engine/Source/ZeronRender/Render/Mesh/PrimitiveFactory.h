// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

#include <Render/Mesh/MeshData.h>

namespace Zeron
{
	namespace Render
	{
		class PrimitiveFactory {
		  public:
			static MeshData CreateCube(float sideLength = 1.f);
			static MeshData CreateSphere(float radius = 0.5f, size_t radialSegments = 12, size_t heightSegments = 12);
			static MeshData CreateSphereIco(size_t subdivisions = 3);
			static MeshData CreatePlane(float width = 1.f, float depth = 1.f, size_t widthSegments = 1, size_t depthSegments = 1);
			static MeshData CreateCapsule(float radius = 0.5f, float height = 1.f, size_t radialSegment = 12, size_t hemisphereSegments = 6, size_t bodySegments = 1);
			static MeshData CreateCylinder(float radius = 0.5f, float height = 1.f, size_t radialSegment = 12, size_t heightSegments = 1);
			static MeshData CreateQuad();
		};
	}
}