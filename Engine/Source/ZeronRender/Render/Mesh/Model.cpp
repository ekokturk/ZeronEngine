// Copyright (C) Eser Kokturk. All Rights Reserved.

#include <Render/Mesh/Model.h>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <Graphics/Buffer.h>
#include <Graphics/Graphics.h>
#include <Graphics/GraphicsContext.h>
#include <Graphics/Primitives.h>
#include <Graphics/Texture.h>
#include <Render/Camera/Camera.h>
#include <Render/Mesh/Mesh.h>


namespace Zeron::Render
{
	Model::Model(Gfx::Graphics& graphics, const ByteBuffer& modelData)
	{
		LoadModel(graphics, modelData);
	}

	std::vector<std::unique_ptr<Mesh>>& Model::GetMeshes()
	{
		return mMeshList;
	}

	const std::vector<std::unique_ptr<Mesh>>& Model::GetMeshes() const
	{
		return mMeshList;
	}

	const Mesh* Model::GetMesh(size_t meshIndex) const
	{
		ZE_ASSERT(meshIndex < mMeshList.size(), "Expected to have a valid mesh");
		return mMeshList[meshIndex].get();
	}

	const Mat4& Model::GetMeshTransform(size_t meshIndex) const
	{
		ZE_ASSERT(meshIndex < mMeshTransforms.size(), "Expected to have a valid mesh transform");
		return mMeshTransforms[meshIndex];
	}

	size_t Model::GetMeshCount() const
	{
		return mMeshList.size();
	}

	bool Model::LoadModel(Gfx::Graphics& graphics, const ByteBuffer& modelData)
	{
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFileFromMemory(modelData.data(), modelData.size(), aiProcess_Triangulate | aiProcess_ConvertToLeftHanded);
		if (scene) {
			ProcessNode(graphics, scene->mRootNode, scene, {});
			return true;
		}
		ZE_FAIL("Unable to read model file!");
		return false;
	}

	void Model::ProcessNode(Gfx::Graphics& graphics, aiNode* node, const aiScene* scene, const Mat4& parentTransform)
	{
		const aiMatrix4x4& m = node->mTransformation;
		const Mat4 transformation = parentTransform * Mat4{ m.a1, m.b1, m.c1, m.d1, m.a2, m.b2, m.c2, m.d2, m.a3, m.b3, m.c3, m.d3, m.a4, m.b4, m.c4, m.d4 };

		for (uint32_t i = 0; i < node->mNumMeshes; ++i) {
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			mMeshList.emplace_back(ProcessMesh(graphics, mesh, scene, transformation));
		}

		for (uint32_t i = 0; i < node->mNumChildren; ++i) {
			ProcessNode(graphics, node->mChildren[i], scene, transformation);
		}
	}

	std::unique_ptr<Mesh> Model::ProcessMesh(Gfx::Graphics& graphics, aiMesh* meshNode, const aiScene* scene, const Mat4& transform)
	{
		std::vector<Gfx::Vertex> vertices;
		std::vector<uint32_t> indices;

		for (unsigned i = 0; i < meshNode->mNumVertices; ++i) {
			Gfx::Vertex vertex;
			vertex.mPosition = { meshNode->mVertices[i].x, meshNode->mVertices[i].y, meshNode->mVertices[i].z };
			vertex.mNormal = { meshNode->mNormals[i].x, meshNode->mNormals[i].y, meshNode->mNormals[i].z };

			// Main texture at 0
			if (meshNode->mTextureCoords[0]) {
				vertex.mTextureCoord = { static_cast<float>(meshNode->mTextureCoords[0][i].x), static_cast<float>(meshNode->mTextureCoords[0][i].y) };
			}

			vertices.push_back(vertex);
		}

		for (unsigned i = 0; i < meshNode->mNumFaces; ++i) {
			const aiFace face = meshNode->mFaces[i];
			for (unsigned j = 0; j < face.mNumIndices; ++j) {
				indices.push_back(face.mIndices[j]);
			}
		}

		// std::vector<std::shared_ptr<Texture>> textures;
		// aiMaterial* material = scene->mMaterials[meshNode->mMaterialIndex];
		// std::vector<std::shared_ptr<Texture>> diffuseTexture = LoadMaterialTextures(graphics, scene, material, TextureType::Diffuse);
		// textures.insert(textures.end(), diffuseTexture.begin(), diffuseTexture.end());

		mMeshTransforms.emplace_back(transform);
		return std::make_unique<Mesh>(graphics, vertices, indices);
	}

	std::vector<std::shared_ptr<Gfx::Texture>> Model::LoadMaterialTextures(Gfx::Graphics& graphics, const aiScene* scene, aiMaterial* material, Gfx::TextureType type)
	{
		std::vector<std::shared_ptr<Gfx::Texture>> materialTextures;
		aiTextureType aiType = aiTextureType::aiTextureType_UNKNOWN;
		switch (type) {
			case Gfx::TextureType::Diffuse: aiType = aiTextureType_DIFFUSE; break;
			case Gfx::TextureType::Normal: aiType = aiTextureType_NORMALS; break;
		}
		const uint32_t textureCount = material->GetTextureCount(aiType);
		if (textureCount == 0) {
			aiColor3D aiColor(0.f);
			switch (type) {
				case Gfx::TextureType::Diffuse: {
					material->Get(AI_MATKEY_COLOR_DIFFUSE, aiColor);
					if (aiColor.IsBlack()) {
						materialTextures.emplace_back(graphics.CreateTexture(type, Color::Pink));
					}
					else {
						materialTextures.emplace_back(graphics.CreateTexture(type, Color(aiColor.r, aiColor.g, aiColor.b)));
					}
				} break;
			}
		}
		else {
			// Get texture paths from the asset
			for (uint32_t i = 0; i < textureCount; ++i) {
				aiString path;
				material->GetTexture(aiType, i, &path);
				std::filesystem::path filePath(path.C_Str());
				// Image texture;
				// texture.Load(filePath.string());
				// materialTextures.emplace_back(graphics.CreateTexture(type, texture.GetColorData().data(), texture.GetWidth(), texture.GetHeight()));
			}
		}

		if (materialTextures.empty()) {
			materialTextures.emplace_back(graphics.CreateTexture(type, Color::Pink));
		}

		return materialTextures;
	}
}