// Copyright (C) Eser Kokturk. All Rights Reserved.

#include <Graphics/Shader.h>

namespace Zeron::Gfx
{
	Shader::Shader(ShaderType type)
		: mType(type)
	{}

	Shader::~Shader() {}

	ShaderType Shader::GetType() const
	{
		return mType;
	}
}