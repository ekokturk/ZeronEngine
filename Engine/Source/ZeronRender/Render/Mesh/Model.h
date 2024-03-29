﻿// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

#include <Graphics/GraphicsTypes.h>
#include <Render/Mesh/Mesh.h>

struct aiMaterial;
struct aiMesh;
struct aiNode;
struct aiScene;

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
			// TODO: Move this to ModelImporter class
			bool LoadModel(Gfx::Graphics& graphics, const ByteBuffer& modelData);
			void ProcessNode(Gfx::Graphics& graphics, aiNode* node, const aiScene* scene, const Mat4& parentTransform);
			std::unique_ptr<Mesh> ProcessMesh(Gfx::Graphics& graphics, aiMesh* meshNode, const aiScene* scene, const Mat4& transform);
			std::vector<std::shared_ptr<Gfx::Texture>> LoadMaterialTextures(Gfx::Graphics& graphics, const aiScene* scene, aiMaterial* material, Gfx::TextureType type);

			std::vector<std::unique_ptr<Mesh>> mMeshList;
			std::vector<Mat4> mMeshTransforms;
		};
	}
}