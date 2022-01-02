// Copyright (C) 2020, Eser Kokturk. All Rights Reserved.

#pragma once

#include "Graphics/GraphicsTypes.h"
#include "Renderer/Mesh.h"

struct aiMaterial;
struct aiMesh;
struct aiNode;
struct aiScene;

namespace Zeron
{
	class Camera;
	class Graphics;
	class Buffer;
	class Texture;
	class GraphicsContext;

	class Model {
	public:
		
		Model(Graphics& graphics, const std::string& modelPath, std::shared_ptr<Buffer> constantBuffer);

		void Draw(GraphicsContext& ctx, Camera& camera);

		std::vector<Mesh>& GetMeshes();
		const std::vector<Mesh>& GetMeshes() const;
		
	private:
		// TODO: Move this to ModelImporter class
		bool LoadModel(Graphics& graphics, const std::string& modelPath);
		void ProcessNode(Graphics& graphics, aiNode* node, const aiScene* scene, const Mat4& parentTransform);
		Mesh ProcessMesh(Graphics& graphics, aiMesh* meshNode, const aiScene* scene, const Mat4& transform);
		std::vector<std::shared_ptr<Texture>> LoadMaterialTextures(Graphics& graphics, const aiScene* scene, aiMaterial* material, TextureType type);
		
		std::vector<Mesh> mMeshList;
		std::shared_ptr<Buffer> mConstantBuffer;
	};
}


