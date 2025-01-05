// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

#include <Graphics/GraphicsTypes.h>
#include <Render/Mesh/Mesh.h>

namespace Zeron
{
	namespace Gfx
	{
		class Buffer;
		class Graphics;
		class GraphicsContext;
		class Texture;
	}

	namespace Render
	{
		class Camera;

		class Model {
		  public:
			Model(Gfx::Graphics& graphics, const ByteBuffer& modelData);

			std::vector<std::unique_ptr<Mesh>>& GetMeshes();
			const std::vector<std::unique_ptr<Mesh>>& GetMeshes() const;

			const Mesh* GetMesh(size_t meshIndex) const;
			const Mat4& GetMeshTransform(size_t meshIndex) const;
			size_t GetMeshCount() const;

		  private:
			std::vector<std::unique_ptr<Mesh>> mMeshList;
			std::vector<Mat4> mMeshTransforms;
		};
	}
}