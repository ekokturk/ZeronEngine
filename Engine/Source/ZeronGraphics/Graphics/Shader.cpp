// Copyright (C) Eser Kokturk. All Rights Reserved.

#include <Graphics/Shader.h>

namespace Zeron
{
	Shader::Shader(ShaderType type)
		: mType(type)
	{}

	Shader::~Shader() {}

	ShaderType Shader::GetType() const
	{
		return mType;
	}

	ShaderProgram::ShaderProgram(const std::string& name, const VertexLayout& vertexLayout, const ResourceLayout& resourceLayout)
		: mName(name)
		, mVertexLayout(vertexLayout)
		, mResourceLayout(resourceLayout)
	{}

	ShaderProgram::~ShaderProgram() {}

	const std::string& ShaderProgram::GetShaderName() const
	{
		return mName;
	}

	const VertexLayout& ShaderProgram::GetVertexLayout() const
	{
		return mVertexLayout;
	}

	const ResourceLayout& ShaderProgram::GetResourceLayout() const
	{
		return mResourceLayout;
	}
}