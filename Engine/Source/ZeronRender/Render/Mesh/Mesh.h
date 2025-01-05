// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

#include <Render/Mesh/MeshData.h>

namespace Zeron
{
	namespace Gfx
	{
		class CommandBuffer;
		class Graphics;
		class Buffer;
	}

	namespace Render
	{
		class Mesh {
		  public:
			Mesh(Gfx::Graphics& graphics, const MeshData& data);
			Mesh(const Mesh& mesh) = default;
			~Mesh();

			const MeshDescription& GetMeshDescription() const;
			uint32_t GetIndexCount() const;

			void Bind(Gfx::CommandBuffer& cmd, std::span<const MeshAttribute> attributes) const;

		  private:
			std::unordered_map<MeshAttribute, std::shared_ptr<Gfx::Buffer>> mVertexBuffers;
			std::shared_ptr<Gfx::Buffer> mIndexBuffer;
			std::shared_ptr<const MeshDescription> mDescription;
		};
	}
}