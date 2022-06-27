// Copyright (C) Eser Kokturk. All Rights Reserved.

#include <Graphics/Shader.h>

namespace Zeron
{
	ShaderType Shader::GetType() const
	{
		return mType;
	}

	Shader* ShaderProgram::GetShader(ShaderType type) const
	{
		switch (type) {
			case ShaderType::Vertex: return mVertexShader.get();
			case ShaderType::Fragment: return mFragmentShader.get();
		}
		return nullptr;
	}

	const std::string& ShaderProgram::GetShaderName() const
	{
		return mName;
	}
}
