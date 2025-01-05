// Copyright (C) Eser Kokturk. All Rights Reserved.

#include <Render/Mesh/Model.h>

#include <Graphics/Buffer.h>
#include <Graphics/Graphics.h>
#include <Graphics/GraphicsContext.h>
#include <Graphics/Primitives.h>
#include <Graphics/Texture.h>
#include <Render/Camera/Camera.h>
#include <Render/Mesh/Mesh.h>
#include <tiny_gltf.h>


namespace Zeron::Render
{
	Model::Model(Gfx::Graphics& graphics, const ByteBuffer& modelData)
	{
		tinygltf::Model gltfModel;
		tinygltf::TinyGLTF loader;
		std::string err;
		std::string warn;

		bool ret = loader.LoadASCIIFromString(&gltfModel, &err, &warn, reinterpret_cast<const char*>(modelData.data()), modelData.size(), "", true);
		if (!warn.empty()) {
			ZE_LOGW("Warning: {}", warn);
		}

		if (!err.empty()) {
			ZE_LOGW("Error: {}", err);
		}

		if (!ret) {
			ZE_LOGW("Failed to load glTF model");
		}

		// Loop through meshes
		for (const tinygltf::Mesh& gltfMesh : gltfModel.meshes) {

			MeshData meshData;

			for (const tinygltf::Primitive& primitive : gltfMesh.primitives) {

				if (primitive.attributes.find("POSITION") != primitive.attributes.end()) {
					const tinygltf::Accessor& accessor = gltfModel.accessors[primitive.attributes.find("POSITION")->second];
					const tinygltf::BufferView& bufferView = gltfModel.bufferViews[accessor.bufferView];
					const tinygltf::Buffer& buffer = gltfModel.buffers[bufferView.buffer];

					auto pos = reinterpret_cast<const float*>(&(buffer.data[bufferView.byteOffset + accessor.byteOffset]));

					for (size_t i = 0; i < accessor.count; ++i) {
						meshData.AppendVertex<MeshAttribute::WorldPos>({ pos[i * 3 + 0], pos[i * 3 + 1], pos[i * 3 + 2] });
					}
				}

				if (primitive.attributes.find("NORMAL") != primitive.attributes.end()) {
					const tinygltf::Accessor& accessor = gltfModel.accessors[primitive.attributes.find("NORMAL")->second];
					const tinygltf::BufferView& bufferView = gltfModel.bufferViews[accessor.bufferView];
					const tinygltf::Buffer& buffer = gltfModel.buffers[bufferView.buffer];

					auto norm = reinterpret_cast<const float*>(&(buffer.data[bufferView.byteOffset + accessor.byteOffset]));

					for (size_t i = 0; i < accessor.count; ++i) {
						meshData.AppendVertex<MeshAttribute::Normal>({ norm[i * 3 + 0], norm[i * 3 + 1], norm[i * 3 + 2] });
					}
				}

				if (primitive.attributes.find("TANGENT") != primitive.attributes.end()) {
					const tinygltf::Accessor& accessor = gltfModel.accessors[primitive.attributes.find("TANGENT")->second];
					const tinygltf::BufferView& bufferView = gltfModel.bufferViews[accessor.bufferView];
					const tinygltf::Buffer& buffer = gltfModel.buffers[bufferView.buffer];

					auto tangent = reinterpret_cast<const float*>(&(buffer.data[bufferView.byteOffset + accessor.byteOffset]));

					for (size_t i = 0; i < accessor.count; ++i) {
						meshData.AppendVertex<MeshAttribute::Tangent>({ tangent[i * 4 + 0], tangent[i * 4 + 1], tangent[i * 4 + 2], tangent[i * 4 + 3] });
					}
				}

				if (primitive.attributes.find("TEXCOORD_0") != primitive.attributes.end()) {
					const tinygltf::Accessor& accessor = gltfModel.accessors[primitive.attributes.find("TEXCOORD_0")->second];
					const tinygltf::BufferView& bufferView = gltfModel.bufferViews[accessor.bufferView];
					const tinygltf::Buffer& buffer = gltfModel.buffers[bufferView.buffer];

					auto uv = reinterpret_cast<const float*>(&(buffer.data[bufferView.byteOffset + accessor.byteOffset]));

					const size_t attrSize = accessor.count;
					for (size_t i = 0; i < accessor.count; ++i) {
						meshData.AppendVertex<MeshAttribute::UV>({ uv[i * 2 + 0], uv[i * 2 + 1] });
					}
				}

				if (primitive.indices >= 0) {
					const tinygltf::Accessor& accessor = gltfModel.accessors[primitive.indices];
					const tinygltf::BufferView& bufferView = gltfModel.bufferViews[accessor.bufferView];
					const tinygltf::Buffer& buffer = gltfModel.buffers[bufferView.buffer];

					if (accessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT) {
						auto indice = reinterpret_cast<const unsigned int*>(&(buffer.data[bufferView.byteOffset + accessor.byteOffset]));
						for (size_t i = 0; i < accessor.count; ++i) {
							meshData.AppendIndex(indice[i]);
						}
					}
					else if (accessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT) {
						const size_t indexCount = accessor.count;
						auto indice = reinterpret_cast<const unsigned short*>(&buffer.data[bufferView.byteOffset + accessor.byteOffset]);
						for (size_t i = 0; i < indexCount; ++i) {
							meshData.AppendIndex(static_cast<uint32_t>(indice[i]));
						}
					}
				}
			}

			// Create empty data
			const size_t positionCount = meshData.GetVertices<MeshAttribute::WorldPos>().size();
			if (auto tangents = meshData.GetVertices<MeshAttribute::Tangent>(); tangents.empty()) {
				std::vector<typename MeshAttributeInfo<MeshAttribute::Tangent>::Type> buffer(positionCount, 0);
				meshData.AppendVertices<MeshAttribute::Tangent>({ buffer });
			}
			if (auto tangents = meshData.GetVertices<MeshAttribute::UV>(); tangents.empty()) {
				std::vector<typename MeshAttributeInfo<MeshAttribute::UV>::Type> buffer(positionCount, 0);
				meshData.AppendVertices<MeshAttribute::UV>({ buffer });
			}

			if (auto tangents = meshData.GetVertices<MeshAttribute::Normal>(); tangents.empty()) {
				std::vector<typename MeshAttributeInfo<MeshAttribute::Normal>::Type> buffer(positionCount, 0);
				meshData.AppendVertices<MeshAttribute::Normal>({ buffer });
			}

			mMeshTransforms.push_back(Mat4());
			mMeshList.push_back(std::make_unique<Mesh>(graphics, meshData));
		}
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
}