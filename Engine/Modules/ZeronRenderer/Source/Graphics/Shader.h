// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once
#include "GraphicsTypes.h"
#include "VertexLayout.h"

namespace Zeron
{
	class Graphics;
	class GraphicsContext;

	class Shader {
	public:
		virtual ~Shader() = default;

		ShaderType GetType() const;

	protected:
		ShaderType mType = ShaderType::Invalid;
	};

	class ShaderProgram {
	public:
		virtual ~ShaderProgram() = default;

		virtual void Bind(GraphicsContext& graphicsContext) = 0;

		Shader* GetShader(ShaderType type) const;
		const std::string& GetShaderName() const;
	
	protected:
		std::string mName;
		std::shared_ptr<Shader> mVertexShader;
		std::shared_ptr<Shader> mFragmentShader;
	};
}
