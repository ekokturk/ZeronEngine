// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

#include <Graphics/GraphicsTypes.h>

namespace Zeron::Gfx
{
	class Shader {
	  public:
		Shader(ShaderType type);
		virtual ~Shader();

		ShaderType GetType() const;

	  protected:
		ShaderType mType;
	};
}