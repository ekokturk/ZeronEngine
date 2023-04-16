// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

#include <Graphics/GraphicsTypes.h>
#include <Graphics/ResourceLayout.h>
#include <Graphics/VertexLayout.h>

namespace Zeron
{
	class ResourceLayout;
	class VertexLayout;
	class Graphics;
	class GraphicsContext;

	class Shader {
	  public:
		Shader(ShaderType type);
		virtual ~Shader();

		ShaderType GetType() const;

	  protected:
		ShaderType mType;
	};

	class ShaderProgram {
	  public:
		ShaderProgram(const std::string& name, const VertexLayout& vertexLayout, const ResourceLayout& resourceLayout);
		virtual ~ShaderProgram();

		const std::string& GetShaderName() const;
		const VertexLayout& GetVertexLayout() const;
		const ResourceLayout& GetResourceLayout() const;

		virtual Shader* GetShader(ShaderType type) const = 0;

	  protected:
		std::string mName;
		VertexLayout mVertexLayout;
		ResourceLayout mResourceLayout;
	};
}