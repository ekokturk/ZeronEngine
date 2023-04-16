// Copyright (C) Eser Kokturk. All Rights Reserved.

#if ZE_GRAPHICS_VULKAN

#	include <Graphics/API/Vulkan/PipelineBindingVulkan.h>

#	include <Graphics/API/Vulkan/BufferVulkan.h>
#	include <Graphics/API/Vulkan/GraphicsVulkan.h>
#	include <Graphics/API/Vulkan/PipelineVulkan.h>
#	include <Graphics/API/Vulkan/SamplerVulkan.h>
#	include <Graphics/API/Vulkan/TextureVulkan.h>

namespace Zeron
{
	PipelineBindingVulkan::PipelineBindingVulkan(GraphicsVulkan& graphics, PipelineVulkan& pipeline, const std::vector<BindingHandle>& bindings)
	{
		const vk::Device& device = graphics.GetDeviceVK();

		const auto& resources = pipeline.GetResourceLayout().GetResources();
		ZE_ASSERT(bindings.size() == resources.size(), "Vulkan pipeline bindings should match resource layout!");

		uint32_t uniformCount = 0, samplerCount = 0, textureCount = 0;
		_countBindings(bindings, uniformCount, samplerCount, textureCount);
		std::vector<vk::DescriptorBufferInfo> bufferInfoList;
		bufferInfoList.reserve(uniformCount);
		std::vector<vk::DescriptorImageInfo> samplerInfoList;
		samplerInfoList.reserve(samplerCount);
		std::vector<vk::DescriptorImageInfo> textureInfoList;
		textureInfoList.reserve(textureCount);

		// WriteDescriptorSet takes info struct as a reference so we have to store them outside of the loop.
		std::vector<vk::WriteDescriptorSet> writeSets;
		writeSets.reserve(bindings.size());

		const auto& descriptorSetLayouts = pipeline.GetDescriptorSetLayouts();
		mDescriptorPool = graphics.CreateDescriptorPoolVK({
			{ vk::DescriptorType::eUniformBuffer, uniformCount },
			{ vk::DescriptorType::eSampler, samplerCount },
			{ vk::DescriptorType::eSampledImage, textureCount },
		});
		mDescriptorSets = device.allocateDescriptorSetsUnique(
			vk::DescriptorSetAllocateInfo(*mDescriptorPool, descriptorSetLayouts.size(), vk::uniqueToRaw(descriptorSetLayouts).data())
		);
		ZE_ASSERT(mDescriptorSets.size() == descriptorSetLayouts.size(), "Vulkan descriptor sets should match pipeline layout!");

		for (size_t i = 0; i < resources.size(); ++i) {
			auto& resource = resources[i];
			auto& binding = bindings[i];
			switch (resource.mType) {
				case PipelineResourceType::UniformBuffer: {
					if (auto* handle = std::get_if<UniformBindingHandle>(&binding)) {
						ZE_ASSERT(handle->mBuffer->GetBufferType() == BufferType::Uniform, "Invalid Vulkan buffer binding type!");
						auto* bufferVk = static_cast<BufferVulkan*>(handle->mBuffer);
						bufferInfoList.emplace_back(bufferVk->GetBufferVK(), 0, bufferVk->GetStride());
						writeSets.emplace_back(
							*mDescriptorSets[resource.mSet], resource.mBinding, 0, 1, vk::DescriptorType::eUniformBuffer, nullptr, &bufferInfoList.back(), nullptr
						);
					}
					else {
						ZE_FAIL("Vulkan pipeline resource binding was expected to be a uniform buffer!");
					}
				} break;
				case PipelineResourceType::Sampler: {
					if (auto* handle = std::get_if<SamplerBindingHandle>(&binding)) {
						auto* samplerVk = static_cast<SamplerVulkan*>(handle->mSampler);
						samplerInfoList.emplace_back(samplerVk->GetSamplerVK(), nullptr, vk::ImageLayout::eShaderReadOnlyOptimal);
						writeSets.emplace_back(
							*mDescriptorSets[resource.mSet], resource.mBinding, 0, 1, vk::DescriptorType::eSampler, &samplerInfoList.back(), nullptr, nullptr
						);
					}
					else {
						ZE_FAIL("Vulkan pipeline resource binding was expected to be a sampler!");
					}
				} break;
				case PipelineResourceType::Texture: {
					if (auto* samplerHandle = std::get_if<TextureBindingHandle>(&binding)) {
						const auto* textureVk = static_cast<TextureVulkan*>(samplerHandle->mTexture);
						textureInfoList.emplace_back(nullptr, textureVk->GetImageViewVK(), vk::ImageLayout::eShaderReadOnlyOptimal);
						writeSets.emplace_back(
							*mDescriptorSets[resource.mSet], resource.mBinding, 0, 1, vk::DescriptorType::eSampledImage, &textureInfoList.back(), nullptr, nullptr
						);
					}
					else {
						ZE_FAIL("Vulkan pipeline resource binding was expected to be a texture!");
					}
				} break;
				case PipelineResourceType::DynamicUniformBuffer:
				default: ZE_FAIL("Vulkan pipeline binding is not supported!");
			}
		}

		device.updateDescriptorSets(writeSets, nullptr);
	}

	uint32_t PipelineBindingVulkan::GetBindingSetCount() const
	{
		return mDescriptorSets.size();
	}

	const std::vector<vk::UniqueDescriptorSet>& PipelineBindingVulkan::GetDescriptorSets() const
	{
		return mDescriptorSets;
	}

	void PipelineBindingVulkan::_countBindings(const std::vector<BindingHandle>& bindings, uint32_t& uniformBuffer, uint32_t& sampler, uint32_t& texture) const
	{
		for (const auto& binding : bindings) {
			std::visit(
				[&uniformBuffer, &sampler, &texture](auto&& arg) {
					using T = std::decay_t<decltype(arg)>;
					if constexpr (std::is_same_v<T, UniformBindingHandle>) {
						++uniformBuffer;
					}
					else if constexpr (std::is_same_v<T, SamplerBindingHandle>) {
						++sampler;
					}
					else if constexpr (std::is_same_v<T, TextureBindingHandle>) {
						++texture;
					}
					else {
						// static_assert(std::always_false_v<T>, "non-exhaustive visitor!");
					}
				},
				binding
			);
		}
	}
}
#endif