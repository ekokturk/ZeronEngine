// Copyright (C) Eser Kokturk. All Rights Reserved.

#include <Graphics/ShaderProgramConfig.h>

#include <cereal/external/rapidjson/document.h>
#include <Graphics/ResourceLayout.h>
#include <Graphics/VertexLayout.h>

namespace Zeron::Gfx
{
	ShaderProgramConfig::ShaderProgramConfig(const std::string& data)
	{
		rapidjson::Document document;
		const auto error = document.Parse(data.c_str()).HasParseError();
		ZE_ASSERT(!error, "Unable to parse shader description");

		if (document.HasMember("name")) {
			mName = document["name"].GetString();
		}

		if (document.HasMember("stages") && document["stages"].IsArray()) {
			const auto& stages = document["stages"];
			for (const auto& stage : stages.GetArray()) {
				mStages.emplace_back(_getShaderType(stage.GetString()));
			}
		}

		if (document.HasMember("layout") && document["layout"].IsObject()) {
			const auto& layout = document["layout"];

			// Parsing vertex layout
			if (layout.HasMember("vertex") && layout["vertex"].IsObject()) {
				const auto& vertexLayout = layout["vertex"];

				if (vertexLayout.HasMember("input") && vertexLayout["input"].IsArray()) {
					for (const auto& input : vertexLayout["input"].GetArray()) {
						Gfx::VertexLayout::Element layoutEl;
						layoutEl.mName = input["semantic"].GetString();
						layoutEl.mFormat = _getVertexFormat(input["type"].GetString());
						if (input.HasMember("instanced")) {
							layoutEl.mIsInstanced = true;
						}
						if (input.HasMember("slot")) {
							layoutEl.mSlot = static_cast<uint8_t>(input["slot"].GetUint());
						}
						mLayout.push_back(std::move(layoutEl));
					}
				}
			}
		}
		if (document.HasMember("bindings") && document["bindings"].IsArray()) {
			const auto& bindings = document["bindings"];
			for (const auto& binding : bindings.GetArray()) {
				Gfx::ResourceLayout::Element layoutEl;
				layoutEl.mShaderStage = _getShaderType(binding["stage"].GetString());
				layoutEl.mType = _getResourceType(binding["bindingType"].GetString());
				layoutEl.mBinding = binding["index"].GetInt();
				layoutEl.mSet = binding["set"].GetInt();
				layoutEl.mTypedBinding = binding["typeIndex"].GetInt();
				mResources.push_back(layoutEl);
			}
		}
		if (document.HasMember("pushConstants") && document["pushConstants"].IsArray()) {
			const auto& pushConstants = document["pushConstants"];
			for (const auto& pushConstant : pushConstants.GetArray()) {
				Gfx::PushConstant::Element pushConstantEl;
				pushConstantEl.mShaderStage = _getShaderType(pushConstant["stage"].GetString());
				pushConstantEl.mTypedBinding = pushConstant["typeIndex"].GetInt();
				pushConstantEl.mStride = pushConstant["stride"].GetInt();
				mPushConstants.push_back(pushConstantEl);
			}
		}
	}
	Gfx::ShaderType ShaderProgramConfig::_getShaderType(const std::string& str) const
	{
		if (str == "vertex") {
			return Gfx::ShaderType::Vertex;
		}
		if (str == "fragment") {
			return Gfx::ShaderType::Fragment;
		}
		if (str == "compute") {
			return Gfx::ShaderType::Compute;
		}

		ZE_FAIL("Unable to find a matching shader type");
		return Gfx::ShaderType::Vertex;
	}
	PipelineResourceType ShaderProgramConfig::_getResourceType(const std::string& str) const
	{
		if (str == "uniform") {
			return Gfx::PipelineResourceType::UniformBuffer;
		}
		if (str == "texture") {
			return Gfx::PipelineResourceType::Texture;
		}
		if (str == "sampler") {
			return Gfx::PipelineResourceType::Sampler;
		}
		if (str == "storageBuffer") {
			return Gfx::PipelineResourceType::StorageBuffer;
		}
		ZE_FAIL("Unable to find a matching shader resource type");
		return Gfx::PipelineResourceType::UniformBuffer;
	}

	VertexFormat ShaderProgramConfig::_getVertexFormat(const std::string& str) const
	{
		if (str == "float2") {
			return Gfx::VertexFormat::Float2;
		}
		if (str == "float3") {
			return Gfx::VertexFormat::Float3;
		}
		if (str == "float4") {
			return Gfx::VertexFormat::Float4;
		}
		if (str == "color") {
			return Gfx::VertexFormat::Color;
		}
		if (str == "uint") {
			return Gfx::VertexFormat::UInt;
		}

		ZE_FAIL("Unable to find a matching vertex format");
		return Gfx::VertexFormat::Float2;
	}
}