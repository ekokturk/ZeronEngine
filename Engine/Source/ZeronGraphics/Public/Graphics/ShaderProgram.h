// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

#include <Graphics/GraphicsTypes.h>
#include <Graphics/ResourceLayout.h>
#include <Graphics/ShaderProgramConfig.h>
#include <Graphics/VertexLayout.h>

namespace Zeron::Gfx
{
	class Shader;

	class ShaderProgram {
	  public:
		ShaderProgram(const ShaderProgramConfig& config);
		virtual ~ShaderProgram();

		const std::string& GetShaderName() const;
		const std::vector<VertexLayout::Element>& GetVertexLayout() const;
		const std::vector<ResourceLayout::Element>& GetResourceLayout() const;
		const std::vector<PushConstant::Element>& GetPushConstants() const;

		virtual Shader* GetShader(ShaderType type) const = 0;
		const PushConstant::Element* GetPushConstant(ShaderType shaderType) const;

	  protected:
		ShaderProgramConfig mConfig;
	};
}