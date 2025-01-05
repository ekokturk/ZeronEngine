// Copyright (C) Eser Kokturk. All Rights Reserved.

#include <Render/Mesh/PrimitiveFactory.h>


namespace Zeron::Render
{
	MeshData PrimitiveFactory::CreateCube(float sideLength)
	{
		// Create vertices from top left corner in clockwise direction for each face
		/*
						 (-0.5,0.5,0.5)              (0.5,0.5,0.5)
								+--------------------+
							   /|			  	    /|
							  / |			       / |
			 (-0.5,0.5,-0.5) +--------------------+ (0.5, 0.5, -0.5 )
							 |  |                 |  |
							 |  |                 |  |
							 |  +(-0.5,-0.5,0.5)--|--+ (0.5,-0.5,0.5)
							 | /				  | /
							 |/                   |/
			(-0.5,-0.5,-0.5) +---------------+ (0.5,-0.5,-0.5)
		*/

		const float len = sideLength;
		const Vec3 positions[] = {
			{ len, len, len },	 { -len, len, len },  { -len, -len, len },	{ len, -len, len },	   // Front
			{ len, len, -len },	 { len, len, len },	  { len, -len, len },	{ len, -len, -len },   // Right
			{ -len, len, -len }, { len, len, -len },  { len, -len, -len },	{ -len, -len, -len },  // Back
			{ -len, len, len },	 { -len, len, -len }, { -len, -len, -len }, { -len, -len, len },   // Left
			{ len, len, -len },	 { -len, len, -len }, { -len, len, len },	{ len, len, len },	   // Top
			{ len, -len, len },	 { -len, -len, len }, { -len, -len, -len }, { len, -len, -len },   // Bottom
		};

		const Vec2 uvs[] = {
			{ 0, 0 }, { 1, 0 }, { 1, 1 }, { 0, 1 },	 // Front face
			{ 0, 0 }, { 1, 0 }, { 1, 1 }, { 0, 1 },	 // Right face
			{ 0, 0 }, { 1, 0 }, { 1, 1 }, { 0, 1 },	 // Back face
			{ 0, 0 }, { 1, 0 }, { 1, 1 }, { 0, 1 },	 // Left face
			{ 0, 0 }, { 1, 0 }, { 1, 1 }, { 0, 1 },	 // Top face
			{ 0, 0 }, { 1, 0 }, { 1, 1 }, { 0, 1 },	 // Top face
		};

		const Vec3 normals[] = {
			{ 0, 0, 1 },  { 0, 0, 1 },	{ 0, 0, 1 },  { 0, 0, 1 },	 // Front
			{ 1, 0, 0 },  { 1, 0, 0 },	{ 1, 0, 0 },  { 1, 0, 0 },	 // Right
			{ 0, 0, -1 }, { 0, 0, -1 }, { 0, 0, -1 }, { 0, 0, -1 },	 // Back
			{ -1, 0, 0 }, { -1, 0, 0 }, { -1, 0, 0 }, { -1, 0, 0 },	 // Left
			{ 0, 1, 0 },  { 0, 1, 0 },	{ 0, 1, 0 },  { 0, 1, 0 },	 // Top
			{ 0, -1, 0 }, { 0, -1, 0 }, { 0, -1, 0 }, { 0, -1, 0 },	 // Top
		};

		const Vec4 tangents[] = {
			{ 1, 0, 0, 1 },	 { 1, 0, 0, 1 },  { 1, 0, 0, 1 },  { 1, 0, 0, 1 },	 // Front
			{ 0, 0, -1, 1 }, { 0, 0, -1, 1 }, { 0, 0, -1, 1 }, { 0, 0, -1, 1 },	 // Right
			{ -1, 0, 0, 1 }, { -1, 0, 0, 1 }, { -1, 0, 0, 1 }, { -1, 0, 0, 1 },	 // Back
			{ 0, 0, 1, 1 },	 { 0, 0, 1, 1 },  { 0, 0, 1, 1 },  { 0, 0, 1, 1 },	 // Left
			{ 1, 0, 0, 1 },	 { 1, 0, 0, 1 },  { 1, 0, 0, 1 },  { 1, 0, 0, 1 },	 // Top
			{ 1, 0, 0, 1 },	 { 1, 0, 0, 1 },  { 1, 0, 0, 1 },  { 1, 0, 0, 1 },	 // Bottom
		};

		const std::array<uint32_t, 36> indices = {
			0,	1,	2,	0,	2,	3,	 // Front
			4,	5,	6,	4,	6,	7,	 // Right
			8,	9,	10, 8,	10, 11,	 // Back
			12, 13, 14, 12, 14, 15,	 // Left
			16, 17, 18, 16, 18, 19,	 // Top
			20, 21, 22, 20, 22, 23,	 // Bottom
		};

		MeshData mesh;
		mesh.AppendVertices<MeshAttribute::WorldPos>(positions);
		mesh.AppendVertices<MeshAttribute::UV>(uvs);
		mesh.AppendVertices<MeshAttribute::Normal>(normals);
		mesh.AppendVertices<MeshAttribute::Tangent>(tangents);
		mesh.AppendIndices(indices);

		return mesh;
	}


	MeshData PrimitiveFactory::CreateSphere(float radius, size_t radialSegments, size_t heightSegments)
	{
		MeshData mesh;
		/*
					 , -  / | \ - ,
				   ,     /  |   \   ,
				 ,------/---|----\--- ,
				,      /    |     \    ,
				,-----|-----|-----|----,
				,      \    |     /    ,
				 ,------\---|----/----,
				   ,     \  |  /    ,
					 ' - ,\ |/_  , '
		*/
		// Standard UV sphere

		const float PI = Math::PI<float>();
		for (size_t i = 0; i <= heightSegments; ++i) {
			const float verticalAngle = PI / 2 - static_cast<float>(i) * (PI / static_cast<float>(heightSegments));
			const float z = radius * cosf(verticalAngle);
			const float y = radius * sinf(verticalAngle);

			for (size_t j = 0; j <= radialSegments; ++j) {
				const float horizontalAngle = static_cast<float>(j) * (2 * PI / static_cast<float>(radialSegments));
				const Vec3 position = { z * cosf(horizontalAngle), y, z * sinf(horizontalAngle) };
				const Vec2 uv = { static_cast<float>(j) / static_cast<float>(radialSegments), static_cast<float>(i) / static_cast<float>(heightSegments) };
				const Vec3 normal = Vec3::Normalize(position);
				const Vec4 tangent = Vec4::Normalize({ -position.Z, 0, position.X, -1 });

				mesh.AppendVertex<MeshAttribute::WorldPos>(position);
				mesh.AppendVertex<MeshAttribute::UV>(uv);
				mesh.AppendVertex<MeshAttribute::Normal>(normal);
				mesh.AppendVertex<MeshAttribute::Tangent>(tangent);
			}
		}

		for (int i = 0; i < heightSegments; ++i) {
			for (int j = 0; j < radialSegments; ++j) {
				const int first = (i * (radialSegments + 1)) + j;
				const int second = first + radialSegments + 1;
				mesh.AppendIndex(first + 1);
				mesh.AppendIndex(second);
				mesh.AppendIndex(first);
				mesh.AppendIndex(first + 1);
				mesh.AppendIndex(second + 1);
				mesh.AppendIndex(second);
			}
		}
		return mesh;
	}

	MeshData PrimitiveFactory::CreateSphereIco(size_t subdivisions)
	{
		/*
						_-_.
					 _-',^. `-_.
				 ._-' ,'   `.   `-_
				!`-_._________`-':::
				!   /\        /\::::
				;  /  \      /..\:::
				! /    \    /....\::
				!/      \  /......\:
				;--.___. \/_.__.--;;
				 '-_    `:!;;;;;;;'
					`-_, :!;;;''
						`-!'
		 */
		// Define icosahedron positions
		const float phi = (1.0f + std::sqrt(5.0f)) / 2.0f;
		std::vector<Vec3> positions = {
			{ -0.5, phi, 0 }, { 0.5, phi, 0 }, { -0.5, -phi, 0 }, { 0.5, -phi, 0 },	 //
			{ 0, -0.5, phi }, { 0, 0.5, phi }, { 0, -0.5, -phi }, { 0, 0.5, -phi },	 //
			{ phi, 0, -0.5 }, { phi, 0, 0.5 }, { -phi, 0, -0.5 }, { -phi, 0, 0.5 }	 //
		};

		for (auto& pos : positions) {
			pos.Normalize();
		}

		// Define icosahedron faces
		std::vector<uint32_t> indices = { 0, 11, 5, 0, 5, 1, 0, 1, 7, 0, 7, 10, 0, 10, 11, 1, 5, 9, 5, 11, 4,  11, 10, 2,  10, 7, 6, 7, 1, 8,
										  3, 9,	 4, 3, 4, 2, 3, 2, 6, 3, 6, 8,	3, 8,  9,  4, 9, 5, 2, 4,  11, 6,  2,  10, 8,  6, 7, 9, 8, 1 };

		// Subdivide faces
		for (size_t div = 0; div < subdivisions; ++div) {
			std::map<std::pair<uint32_t, uint32_t>, uint32_t> midpointCache;
			auto getMidpoint = [&](uint32_t i1, uint32_t i2) -> uint32_t {
				const auto key = std::minmax(i1, i2);
				const auto itr = midpointCache.find(key);
				if (itr != midpointCache.end()) {
					return itr->second;
				}

				const uint32_t index = positions.size();
				positions.push_back(((positions[i1] + positions[i2]) * 0.5f).Normalize());
				midpointCache[key] = index;
				return index;
			};

			std::vector<uint32_t> newIndices;
			for (size_t i = 0; i < indices.size(); i += 3) {
				uint32_t i1 = indices[i];
				uint32_t i2 = indices[i + 1];
				uint32_t i3 = indices[i + 2];

				uint32_t a = getMidpoint(i1, i2);
				uint32_t b = getMidpoint(i2, i3);
				uint32_t c = getMidpoint(i3, i1);
				newIndices.insert(newIndices.end(), { i1, a, c, i2, b, a, i3, c, b, a, b, c });
			}

			indices = newIndices;
		}

		// Calculate Normals and UVs
		std::vector<Vec2> uvs;
		std::vector<Vec3> normals;
		for (auto& position : positions) {
			const float u = 0.5f + (std::atan2(position.Z, position.X) / (2.0f * Math::PI<float>()));
			const float v = 0.5f - (std::asin(position.Y) / Math::PI<float>());
			uvs.emplace_back(u, v);
			normals.emplace_back(position);
		}

		// We need to do additional processing to fix warped UVs here
		// TODO: FixWarpedUV
		// TODO: FixUVPole

		// Calculate tangents and bi-normals
		std::vector<Vec4> tangents(positions.size(), 0);
		for (size_t i = 0; i < indices.size(); i += 3) {
			uint32_t i1 = indices[i];
			uint32_t i2 = indices[i + 1];
			uint32_t i3 = indices[i + 2];

			Vec3 edge1 = positions[i2] - positions[i1];
			Vec3 edge2 = positions[i3] - positions[i1];
			Vec2 deltaUV1 = uvs[i2] - uvs[i1];
			Vec2 deltaUV2 = uvs[i3] - uvs[i1];

			float f = 1.0f / (deltaUV1.X * deltaUV2.Y - deltaUV2.X * deltaUV1.Y);
			Vec3 tangent = ((edge1 * deltaUV2.Y - edge2 * deltaUV1.Y) * f).Normalize();

			const Vec3 biTangent = Vec3::Cross(normals[i1], tangent);
			const float biTangentSign = Vec3::Dot(biTangent, edge2) < 0.0f ? -1.0f : 1.0f;

			tangents[i1] = Vec4{ tangent.X, tangent.Y, tangent.Z, biTangentSign }.Normalize();
			tangents[i2] = Vec4{ tangent.X, tangent.Y, tangent.Z, biTangentSign }.Normalize();
			tangents[i3] = Vec4{ tangent.X, tangent.Y, tangent.Z, biTangentSign }.Normalize();
		}

		MeshData mesh;
		mesh.AppendVertices<MeshAttribute::WorldPos>(positions);
		mesh.AppendVertices<MeshAttribute::UV>(uvs);
		mesh.AppendVertices<MeshAttribute::Normal>(normals);
		mesh.AppendVertices<MeshAttribute::Tangent>(tangents);
		mesh.AppendIndices(indices);
		return mesh;
	}

	MeshData PrimitiveFactory::CreatePlane(float width, float depth, size_t widthSegments, size_t depthSegments)
	{
		MeshData mesh;
		/*
			  (-0.5, 0, 0.5)   (0.5, 0, 0.5)
					/-------------/
				   /             /
				  /             /
				 /             /
				/-------------/
		(-0.5, 0, -0.5)    (0.5, 0, -0.5)

		 */

		const float segmentWidth = width / widthSegments;
		const float segmentDepth = depth / depthSegments;
		const float startX = width / 2.0f;
		const float startZ = -depth / 2.0f;

		for (size_t i = 0; i <= depthSegments; ++i) {
			float z = startZ + static_cast<float>(i) * segmentDepth;
			float v = static_cast<float>(i) / depthSegments;

			for (size_t j = 0; j <= widthSegments; ++j) {
				float x = startX - static_cast<float>(j) * segmentWidth;
				float u = static_cast<float>(j) / widthSegments;

				mesh.AppendVertex<MeshAttribute::WorldPos>({ x, 0.f, z });
				mesh.AppendVertex<MeshAttribute::UV>({ u, v });
				mesh.AppendVertex<MeshAttribute::Normal>({ 0.f, 1.f, 0.f });
				mesh.AppendVertex<MeshAttribute::Tangent>({ 1.f, 0.f, 0.f, -1.f });
			}
		}

		for (size_t i = 0; i < depthSegments; ++i) {
			for (size_t j = 0; j < widthSegments; ++j) {
				const uint32_t start = i * (widthSegments + 1u) + j;
				const uint32_t nextRow = start + widthSegments + 1u;

				mesh.AppendIndex(start + 1);
				mesh.AppendIndex(nextRow);
				mesh.AppendIndex(start);
				mesh.AppendIndex(nextRow + 1);
				mesh.AppendIndex(nextRow);
				mesh.AppendIndex(start + 1);
			}
		}

		return mesh;
	}


	MeshData PrimitiveFactory::CreateCapsule(float radius, float height, size_t radialSegment, size_t hemisphereSegments, size_t bodySegments)
	{
		MeshData mesh;
		const float halfHeight = height / 2.0f;
		const float totalHeight = height + 2 * radius;
		const float horizontalStep = 2 * Math::PI<float>() / radialSegment;
		const float verticalStep = Math::PI<float>() / hemisphereSegments;

		// Top
		for (size_t i = 0; i <= hemisphereSegments; ++i) {
			const float phi = i * verticalStep / 2.0f;
			const float y = halfHeight + radius * cos(phi);
			const float v = 1.0f - (y + halfHeight + radius) / totalHeight;

			for (size_t j = 0; j <= radialSegment; ++j) {
				const float theta = j * horizontalStep;
				const float x = radius * sin(phi) * cos(theta);
				const float z = radius * sin(phi) * sin(theta);
				const float u = static_cast<float>(j) / radialSegment;

				mesh.AppendVertex<MeshAttribute::WorldPos>({ x, y, z });
				mesh.AppendVertex<MeshAttribute::UV>({ u, v });
				mesh.AppendVertex<MeshAttribute::Normal>(Vec3{ x, radius * std::cos(phi), z }.Normalize());
				Vec3 tangent = Vec3{ -std::sin(theta), 0.0f, std::cos(theta) }.Normalize();
				mesh.AppendVertex<MeshAttribute::Tangent>({ tangent.X, tangent.Y, tangent.Z, -1 });
			}
		}

		// Middle
		for (size_t i = 0; i <= bodySegments; ++i) {
			const float y = -halfHeight + (i * height) / bodySegments;
			const float v = 1.0f - (y + halfHeight + radius) / totalHeight;

			for (size_t j = 0; j <= radialSegment; ++j) {
				const float theta = j * horizontalStep;
				const float x = radius * std::cos(theta);
				const float z = radius * std::sin(theta);
				const float u = static_cast<float>(j) / radialSegment;

				mesh.AppendVertex<MeshAttribute::WorldPos>({ x, y, z });
				mesh.AppendVertex<MeshAttribute::UV>({ u, v });
				mesh.AppendVertex<MeshAttribute::Normal>({ x / radius, 0, z / radius });
				Vec3 tangent = Vec3{ -z, 0, x }.Normalize();
				mesh.AppendVertex<MeshAttribute::Tangent>({ tangent.X, tangent.Y, tangent.Z, -1 });
			}
		}

		// Bottom
		for (size_t i = 0; i <= hemisphereSegments; ++i) {
			const float phi = Math::PI<float>() / 2.0f + i * verticalStep / 2.0f;
			const float y = -halfHeight + radius * cos(phi);
			const float v = 1.0f - (y + halfHeight + radius) / totalHeight;

			for (size_t j = 0; j <= radialSegment; ++j) {
				const float theta = j * horizontalStep;
				const float x = radius * std::sin(phi) * std::cos(theta);
				const float z = radius * std::sin(phi) * std::sin(theta);
				const float u = static_cast<float>(j) / radialSegment;

				mesh.AppendVertex<MeshAttribute::WorldPos>({ x, y, z });
				mesh.AppendVertex<MeshAttribute::UV>({ u, v });
				Vec3 normal = Vec3{ x, -radius * std::cos(phi), z }.Normalize();
				mesh.AppendVertex<MeshAttribute::Normal>(normal);
				Vec3 tangent = Vec3{ -std::sin(theta), 0.0f, std::cos(theta) }.Normalize();
				mesh.AppendVertex<MeshAttribute::Tangent>({ tangent.X, tangent.Y, tangent.Z, -1 });
			}
		}

		// Indices
		auto connectSegments = [&](size_t startOffset, size_t verticalSegments) {
			for (size_t i = 0; i < verticalSegments; ++i) {
				const uint32_t verticalStart = startOffset + i * (radialSegment + 1);
				const uint32_t nextVerticalStart = startOffset + (i + 1) * (radialSegment + 1);

				for (uint32_t j = 0; j < radialSegment; ++j) {
					mesh.AppendIndex(verticalStart + j + 1);
					mesh.AppendIndex(nextVerticalStart + j);
					mesh.AppendIndex(verticalStart + j);

					mesh.AppendIndex(nextVerticalStart + j + 1);
					mesh.AppendIndex(nextVerticalStart + j);
					mesh.AppendIndex(verticalStart + j + 1);
				}
			}
		};

		const size_t topHemisphereOffset = 0;
		const size_t middleCylinderOffset = (hemisphereSegments + 1) * (radialSegment + 1);
		const size_t bottomHemisphereOffset = middleCylinderOffset + (bodySegments + 1) * (radialSegment + 1);

		connectSegments(topHemisphereOffset, hemisphereSegments);
		connectSegments(middleCylinderOffset, hemisphereSegments);
		connectSegments(bottomHemisphereOffset, hemisphereSegments);

		return mesh;
	}

	MeshData PrimitiveFactory::CreateCylinder(float radius, float height, size_t radialSegment, size_t heightSegments)
	{
		MeshData mesh;
		/*
							(0, 0.5, 0.5)
							  _.-----._
			 (-0.5, 0.5, 0) .-         -. (0.5, 0.5, 0)
							|-_       _-|
							|  ~-----~  |
							|           |
							`._       _.'
							   "-----"
							(0, -0.5, -0.5)
		 */

		const float horizontalStep = 2 * Math::PI<float>() / radialSegment;
		const float verticalStep = height / heightSegments;

		// Middle
		for (size_t i = 0; i <= heightSegments; ++i) {
			const float y = -height / 2 + i * verticalStep;
			const float v = 1.0f - static_cast<float>(i) / heightSegments;

			for (size_t j = 0; j <= radialSegment; ++j) {
				const float theta = j * horizontalStep;
				const float x = radius * std::cos(theta);
				const float z = radius * std::sin(theta);
				const float u = (static_cast<float>(j) * 2 / radialSegment) - (j < radialSegment / 2 ? 0.f : 1.f);

				mesh.AppendVertex<MeshAttribute::WorldPos>({ x, y, z });
				mesh.AppendVertex<MeshAttribute::UV>({ u, v });
				mesh.AppendVertex<MeshAttribute::Normal>({ x, 0, z });
				Vec3 tangent = Vec3{ -z, 0, x }.Normalize();
				mesh.AppendVertex<MeshAttribute::Tangent>({ tangent.X, tangent.Y, tangent.Z, -1 });
			}
		}

		for (size_t i = 0; i < heightSegments; ++i) {
			const uint32_t verticalStart = i * (radialSegment + 1);
			const uint32_t nextVerticalStart = (i + 1) * (radialSegment + 1);

			for (uint32_t j = 0; j < radialSegment; ++j) {
				mesh.AppendIndex(verticalStart + j);
				mesh.AppendIndex(nextVerticalStart + j);
				mesh.AppendIndex(verticalStart + j + 1);

				mesh.AppendIndex(verticalStart + j + 1);
				mesh.AppendIndex(nextVerticalStart + j);
				mesh.AppendIndex(nextVerticalStart + j + 1);
			}
		}

		// Top
		const uint32_t topCenterIndex = mesh.GetVertices<MeshAttribute::WorldPos>().size();
		mesh.AppendVertex<MeshAttribute::WorldPos>({ 0, height / 2, 0 });
		mesh.AppendVertex<MeshAttribute::UV>({ 0.5, 0.5 });
		mesh.AppendVertex<MeshAttribute::Normal>({ 0, 1, 0 });
		mesh.AppendVertex<MeshAttribute::Tangent>({ 1, 0, 0, -1 });

		for (uint32_t j = 0; j <= radialSegment; ++j) {
			const float theta = j * horizontalStep;
			const float x = radius * std::cos(theta);
			const float z = radius * std::sin(theta);

			mesh.AppendVertex<MeshAttribute::WorldPos>({ x, height / 2, z });
			mesh.AppendVertex<MeshAttribute::UV>({ 0.5f - x / (2 * radius), 0.5f + z / (2 * radius) });
			mesh.AppendVertex<MeshAttribute::Normal>({ 0, 1, 0 });
			mesh.AppendVertex<MeshAttribute::Tangent>({ -z, 0, x, -1 });
		}

		const uint32_t topCapStart = topCenterIndex + 1;
		for (unsigned int j = 0; j < radialSegment; ++j) {
			mesh.AppendIndex(topCapStart + (j + 1));
			mesh.AppendIndex(topCapStart + j);
			mesh.AppendIndex(topCenterIndex);
		}


		// Bottom
		const uint32_t bottomCenterIndex = mesh.GetVertices<MeshAttribute::WorldPos>().size();
		mesh.AppendVertex<MeshAttribute::WorldPos>({ 0, -height / 2, 0 });
		mesh.AppendVertex<MeshAttribute::UV>({ 0.5f, 0.5f });
		mesh.AppendVertex<MeshAttribute::Normal>({ 0, -1, 0 });
		mesh.AppendVertex<MeshAttribute::Tangent>({ 1, 0, 0, -1 });

		for (size_t j = 0; j <= radialSegment; ++j) {
			const float theta = j * horizontalStep;
			const float x = radius * std::cos(theta);
			const float z = radius * std::sin(theta);

			mesh.AppendVertex<MeshAttribute::WorldPos>({ x, -height / 2, z });
			mesh.AppendVertex<MeshAttribute::UV>({ 0.5f + x / (2 * radius), 0.5f + z / (2 * radius) });
			mesh.AppendVertex<MeshAttribute::Normal>({ 0, -1, 0 });
			mesh.AppendVertex<MeshAttribute::Tangent>({ z, 0, -x, -1 });
		}

		const uint32_t bottomCapStart = bottomCenterIndex + 1;
		for (unsigned int j = 0; j < radialSegment; ++j) {
			mesh.AppendIndex(bottomCapStart + j);
			mesh.AppendIndex(bottomCapStart + j + 1);
			mesh.AppendIndex(bottomCenterIndex);
		}
		return mesh;
	}

	MeshData PrimitiveFactory::CreateQuad()
	{
		MeshData mesh;

		const Vec3 positions[] = {
			{ -1.0f, -1.0f, 0.0f },
			{ 1.0f, -1.0f, 0.0f },
			{ 1.0f, 1.0f, 0.0f },
			{ -1.0f, 1.0f, 0.0f },
		};

		const Vec2 uvs[] = {
			{ 0.0f, 1.0f },
			{ 1.0f, 1.0f },
			{ 1.0f, 0.0f },
			{ 0.0f, 0.0f },
		};

		const uint32_t indices[] = { 0, 1, 2, 0, 2, 3 };

		mesh.AppendVertices<MeshAttribute::WorldPos>(positions);
		mesh.AppendVertices<MeshAttribute::UV>(uvs);
		mesh.AppendIndices(indices);

		return mesh;
	}
}