// Copyright (C) Eser Kokturk. All Rights Reserved.

#include <Graphics/ShaderProgram.h>

namespace Zeron::Gfx
{
	ShaderProgram::ShaderProgram(const ShaderProgramConfig& config)
		: mConfig(config)
	{}

	ShaderProgram::~ShaderProgram() {}

	const std::string& ShaderProgram::GetShaderName() const
	{
		return mConfig.mName;
	}

	const std::vector<VertexLayout::Element>& ShaderProgram::GetVertexLayout() const
	{
		return mConfig.mLayout;
	}

	const std::vector<ResourceLayout::Element>& ShaderProgram::GetResourceLayout() const
	{
		return mConfig.mResources;
	}

	const std::vector<PushConstant::Element>& ShaderProgram::GetPushConstants() const
	{
		return mConfig.mPushConstants;
	}

	const PushConstant::Element* ShaderProgram::GetPushConstant(ShaderType shaderType) const
	{
		const auto found = std::find_if(GetPushConstants().begin(), GetPushConstants().end(), [shaderType](const PushConstant::Element& item) {
			return item.mShaderStage == shaderType;
		});
		return found != GetPushConstants().end() ? &(*found) : nullptr;
	}
}