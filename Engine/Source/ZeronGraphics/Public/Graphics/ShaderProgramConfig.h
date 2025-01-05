// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

#include <Graphics/GraphicsTypes.h>
#include <Graphics/PushConstant.h>
#include <Graphics/ResourceLayout.h>
#include <Graphics/VertexLayout.h>

namespace Zeron::Gfx
{
	struct ShaderProgramConfig {

		ShaderProgramConfig() = default;
		ShaderProgramConfig(const std::string& data);

		std::string mName;
		std::vector<ShaderType> mStages;
		std::vector<VertexLayout::Element> mLayout;
		std::vector<ResourceLayout::Element> mResources;
		std::vector<PushConstant::Element> mPushConstants;

	  private:
		ShaderType _getShaderType(const std::string& str) const;
		PipelineResourceType _getResourceType(const std::string& str) const;
		VertexFormat _getVertexFormat(const std::string& str) const;
	};
}