// Copyright (C) Eser Kokturk. All Rights Reserved.

#if ZE_GRAPHICS_VULKAN

#	include <Graphics/API/Vulkan/CommandBufferVulkan.h>

#	include <Graphics/API/Vulkan/VulkanHelpers.h>
#	include <Graphics/API/Vulkan/BufferVulkan.h>
#	include <Graphics/API/Vulkan/FrameBufferVulkan.h>
#	include <Graphics/API/Vulkan/GraphicsVulkan.h>
#	include <Graphics/API/Vulkan/PipelineBindingVulkan.h>
#	include <Graphics/API/Vulkan/PipelineVulkan.h>
#	include <Graphics/API/Vulkan/RenderPassVulkan.h>
#	include <Graphics/API/Vulkan/TextureVulkan.h>

namespace Zeron::Gfx
{
	CommandBufferVulkan::CommandBufferVulkan(GraphicsVulkan& graphics, uint32_t count, bool isCompute)
		: mDevice(graphics.GetDeviceVK())
		, mCommandPool(graphics.CreateCommandPoolVK())
		, mCurrentCommandBufferIndex(0)
		, mCurrentRenderPass(nullptr)
		, mCurrentFrameBuffer(nullptr)
		, mCurrentPipeline(nullptr)
	{
		const vk::CommandBufferAllocateInfo allocateInfo(*mCommandPool, vk::CommandBufferLevel::ePrimary, count);
		mCommandBufferList = graphics.GetDeviceVK().allocateCommandBuffersUnique(allocateInfo);
	}

	CommandBufferVulkan::~CommandBufferVulkan() {}

	void CommandBufferVulkan::Begin()
	{
		_acquireNextBuffer();
		const vk::CommandBufferBeginInfo beginInfo{ vk::CommandBufferUsageFlagBits::eOneTimeSubmit, nullptr };
		_getCurrent().reset(vk::CommandBufferResetFlagBits::eReleaseResources);
		_getCurrent().begin(beginInfo);
	}

	void CommandBufferVulkan::BeginRenderPass(FrameBuffer* frameBuffer)
	{
		mCurrentFrameBuffer = static_cast<FrameBufferVulkan*>(frameBuffer);
		ZE_ASSERT(mCurrentFrameBuffer, "CommandBufferVulkan: Cannot have null frame buffer!");
		mCurrentRenderPass = &mCurrentFrameBuffer->GetRenderPass();
		ZE_ASSERT(mCurrentRenderPass, "CommandBufferVulkan: Cannot have null render pass!");

		const Vec2i& extent = mCurrentFrameBuffer->GetExtent();
		ZE_ASSERT(mScissor.extent.width <= extent.X && mScissor.extent.height <= extent.Y, "CommandBufferVulkan: Render pass has to be within frame buffer extent!");

		const vk::RenderPassBeginInfo beginInfo(
			mCurrentRenderPass->GetRenderPassVK(), mCurrentFrameBuffer->GetFrameBufferVK(), mScissor, static_cast<uint32_t>(mClearValue.size()), mClearValue.data()
		);
		_getCurrent().beginRenderPass(&beginInfo, vk::SubpassContents::eInline);
	}

	void CommandBufferVulkan::EndRenderPass()
	{
		_getCurrent().endRenderPass();
		mCurrentRenderPass = nullptr;
		mCurrentFrameBuffer = nullptr;
	}

	void CommandBufferVulkan::End()
	{
		_getCurrent().end();
	}

	void CommandBufferVulkan::Clear(Color color)
	{
		mClearValue = {
			vk::ClearDepthStencilValue{ 1.0f, 0 },
			vk::ClearColorValue(std::array<float, 4>{ color.normR(), color.normG(), color.normB(), color.normA() }),
		};
	}

	void CommandBufferVulkan::SetScissor(const Vec2i& extent, const Vec2i& offset)
	{
		mScissor = vk::Rect2D({ offset.X, offset.Y }, { static_cast<uint32_t>(extent.X), static_cast<uint32_t>(extent.Y) });
		_getCurrent().setScissor(0, 1, &mScissor);
	}

	void CommandBufferVulkan::SetViewport(const Vec2i& size, const Vec2i& position)
	{
		// Flipping viewport to correct axises
		const vk::Viewport info(static_cast<float>(position.X), static_cast<float>(position.Y + size.Y), static_cast<float>(size.X), static_cast<float>(-size.Y), 0.0f, 1.0f);
		_getCurrent().setViewport(0, 1, &info);
	}

	void CommandBufferVulkan::SetPipeline(Pipeline& pipeline)
	{
		mCurrentPipeline = static_cast<PipelineVulkan*>(&pipeline);
		_getCurrent().bindPipeline(mCurrentPipeline->IsCompute() ? vk::PipelineBindPoint::eCompute : vk::PipelineBindPoint::eGraphics, mCurrentPipeline->GetPipelineVK());
	}

	void CommandBufferVulkan::SetPipelineBinding(PipelineBinding& binding, uint32_t index)
	{
		ZE_ASSERT(mCurrentPipeline, "CommandBufferVulkan: Cannot set pipeline layout without an existing pipeline!");
		const auto& bindingVk = static_cast<PipelineBindingVulkan&>(binding);
		const auto& descriptorSets = bindingVk.GetDescriptorSets();
		_getCurrent().bindDescriptorSets(
			mCurrentPipeline->IsCompute() ? vk::PipelineBindPoint::eCompute : vk::PipelineBindPoint::eGraphics,
			mCurrentPipeline->GetPipelineLayoutVK(),
			index,
			static_cast<uint32_t>(descriptorSets.size()),
			vk::uniqueToRaw(descriptorSets).data(),
			0,
			nullptr
		);
	}

	void CommandBufferVulkan::SetVertexBuffer(Buffer& vb, uint32_t slot)
	{
		ZE_ASSERT(vb.GetBufferType() == BufferType::Vertex, "CommandBufferVulkan: Command expected vertex buffer!");
		auto& bufferVk = static_cast<BufferVulkan&>(vb);
		_getCurrent().bindVertexBuffers(slot, bufferVk.GetBufferVK(), { 0 });
	}

	void CommandBufferVulkan::SetVertexBuffers(Buffer** vb, uint32_t count, uint32_t slot)
	{
		std::vector<vk::Buffer> buffers(count);
		std::vector<vk::DeviceSize> offsets(count);
		for (uint32_t i = 0; i < count; ++i) {
			auto* bufferVk = static_cast<BufferVulkan*>(vb[i]);
			ZE_ASSERT(bufferVk->GetBufferType() == BufferType::Vertex, "CommandBufferVulkan: Command expected vertex buffer!");
			buffers[i] = bufferVk->GetBufferVK();
			offsets[i] = 0;
		}
		_getCurrent().bindVertexBuffers(slot, count, buffers.data(), offsets.data());
	}

	void CommandBufferVulkan::SetIndexBuffer(Buffer& ib)
	{
		ZE_ASSERT(ib.GetBufferType() == BufferType::Index, "CommandBufferVulkan: Command expected index buffer!");
		ZE_ASSERT(ib.GetStride() == 2 || ib.GetStride() == 4, "CommandBufferVulkan: Unsupported index buffer stride!");
		auto& bufferVk = static_cast<BufferVulkan&>(ib);
		_getCurrent().bindIndexBuffer(bufferVk.GetBufferVK(), 0, ib.GetStride() == 2 ? vk::IndexType::eUint16 : vk::IndexType::eUint32);
	}

	void CommandBufferVulkan::CopyBuffer(Buffer& source, Buffer& destination)
	{
		ZE_ASSERT(source.GetSizeInBytes() == destination.GetSizeInBytes(), "CommandBufferVulkan: Buffer sizes do not match for copy command!");
		auto& sourceVk = static_cast<BufferVulkan&>(source);
		auto& destinationVk = static_cast<BufferVulkan&>(destination);
		const vk::BufferCopy copyRegion(0, 0, source.GetSizeInBytes());
		_getCurrent().copyBuffer(sourceVk.GetBufferVK(), destinationVk.GetBufferVK(), copyRegion);
	}

	void CommandBufferVulkan::UpdateBuffer(Buffer& buff, const void* data, uint32_t sizeBytes, uint32_t offset, BufferUpdateRule updateRule)
	{
		auto& bufferVk = static_cast<BufferVulkan&>(buff);
		bufferVk.UpdateVK(mDevice, data, sizeBytes, offset, updateRule);
	}

	void CommandBufferVulkan::SetPushConstant(const void* data, uint32_t stride, ShaderType shader)
	{
		ZE_ASSERT(
			mCurrentPipeline && mCurrentPipeline->GetPushConstant(shader) && mCurrentPipeline->GetPushConstant(shader)->mStride >= stride,
			"CommandBufferVulkan: Expected to have a valid push constant defined for the pipeline"
		);
		_getCurrent().pushConstants(mCurrentPipeline->GetPipelineLayoutVK(), VulkanHelpers::GetShaderStage(shader), 0, stride, data);
	}

	void CommandBufferVulkan::Draw(uint32_t vertexCount, uint32_t vertexStart)
	{
		_getCurrent().draw(vertexCount, 1, vertexStart, 0);
	}

	void CommandBufferVulkan::DrawIndexed(uint32_t indexCount, uint32_t indexStart, uint32_t vertexStart)
	{
		_getCurrent().drawIndexed(indexCount, 1, indexStart, vertexStart, 0);
	}

	void CommandBufferVulkan::DrawInstanced(uint32_t vertexCount, uint32_t instanceCount, uint32_t vertexStart, uint32_t instanceStart)
	{
		_getCurrent().draw(vertexCount, instanceCount, vertexStart, instanceStart);
	}

	void CommandBufferVulkan::DrawInstancedIndexed(uint32_t indexCount, uint32_t instanceCount, uint32_t indexStart, uint32_t baseVertexLocation, uint32_t instanceStart)
	{
		_getCurrent().drawIndexed(indexCount, instanceCount, indexStart, baseVertexLocation, instanceStart);
	}

	void CommandBufferVulkan::Dispatch(uint32_t countX, uint32_t countY, uint32_t countZ)
	{
		_getCurrent().dispatch(countX, countY, countZ);
	}

	void CommandBufferVulkan::AddBarrier(Texture& texture, TextureLayout oldLayout, TextureLayout newLayout)
	{
		TextureVulkan& textureVk = static_cast<TextureVulkan&>(texture);
		ApplyImageTransitionLayoutVK(textureVk, oldLayout, newLayout);
	}

	uint32_t CommandBufferVulkan::GetBufferCount() const
	{
		return static_cast<uint32_t>(mCommandBufferList.size());
	}

	void CommandBufferVulkan::BeginDebugGroup(std::string_view label) const
	{
#	if ZE_DEBUG && !ZE_PLATFORM_ANDROID
		const vk::DebugUtilsLabelEXT debugLabel{ label.data() };
		_getCurrent().beginDebugUtilsLabelEXT(debugLabel);
#	endif
	}

	void CommandBufferVulkan::EndDebugGroup() const
	{
#	if ZE_DEBUG && !ZE_PLATFORM_ANDROID
		_getCurrent().endDebugUtilsLabelEXT();
#	endif
	}

	vk::CommandBuffer& CommandBufferVulkan::GetCommandBufferVK()
	{
		return _getCurrent();
	}

	void CommandBufferVulkan::ApplyImageTransitionLayoutVK(TextureVulkan& texture, TextureLayout oldLayout, TextureLayout newLayout)
	{
		vk::ImageMemoryBarrier barrier;
		barrier.image = texture.GetImageVK();
		barrier.oldLayout = VulkanHelpers::GetTextureLayout(oldLayout);
		barrier.newLayout = VulkanHelpers::GetTextureLayout(newLayout);
		barrier.subresourceRange.aspectMask = vk::ImageAspectFlagBits::eColor;
		barrier.subresourceRange.baseMipLevel = 0;
		barrier.subresourceRange.levelCount = texture.GetMipLevelVK();
		barrier.subresourceRange.baseArrayLayer = 0;
		barrier.subresourceRange.layerCount = 1;

		if (oldLayout == TextureLayout::Undefined) {
			if (newLayout == TextureLayout::ColorAttachment) {
				barrier.dstAccessMask = vk::AccessFlagBits::eColorAttachmentRead | vk::AccessFlagBits::eColorAttachmentWrite;
				_setImageMemoryBarrierVK(barrier, vk::PipelineStageFlagBits::eTopOfPipe, vk::PipelineStageFlagBits::eColorAttachmentOutput);
				return;
			}
			if (newLayout == TextureLayout::DepthStencilAttachment) {
				barrier.dstAccessMask = vk::AccessFlagBits::eDepthStencilAttachmentRead | vk::AccessFlagBits::eDepthStencilAttachmentWrite;
				barrier.subresourceRange.aspectMask = vk::ImageAspectFlagBits::eDepth | vk::ImageAspectFlagBits::eStencil;
				_setImageMemoryBarrierVK(barrier, vk::PipelineStageFlagBits::eTopOfPipe, vk::PipelineStageFlagBits::eEarlyFragmentTests);
				return;
			}
			if (newLayout == TextureLayout::TransferDst) {
				barrier.dstAccessMask = vk::AccessFlagBits::eTransferWrite;
				_setImageMemoryBarrierVK(barrier, vk::PipelineStageFlagBits::eTopOfPipe, vk::PipelineStageFlagBits::eTransfer);
				return;
			}
		}
		else if (oldLayout == TextureLayout::TransferDst) {
			if (newLayout == TextureLayout::ShaderReadOnly) {
				barrier.srcAccessMask = vk::AccessFlagBits::eTransferWrite;
				barrier.dstAccessMask = vk::AccessFlagBits::eShaderRead;
				_setImageMemoryBarrierVK(barrier, vk::PipelineStageFlagBits::eTransfer, vk::PipelineStageFlagBits::eFragmentShader);
				return;
			}
		}
		else if (oldLayout == TextureLayout::DepthStencilAttachment) {
			if (newLayout == TextureLayout::ShaderReadOnly) {
				barrier.subresourceRange.aspectMask = vk::ImageAspectFlagBits::eDepth | vk::ImageAspectFlagBits::eStencil;
				barrier.srcAccessMask = vk::AccessFlagBits::eDepthStencilAttachmentRead | vk::AccessFlagBits::eDepthStencilAttachmentWrite;
				barrier.dstAccessMask = vk::AccessFlagBits::eShaderRead;
				_setImageMemoryBarrierVK(barrier, vk::PipelineStageFlagBits::eLateFragmentTests, vk::PipelineStageFlagBits::eFragmentShader);
				return;
			}
		}
		else if (oldLayout == TextureLayout::ShaderReadOnly) {
			if (newLayout == TextureLayout::DepthStencilAttachment) {
				barrier.subresourceRange.aspectMask = vk::ImageAspectFlagBits::eDepth | vk::ImageAspectFlagBits::eStencil;
				barrier.srcAccessMask = vk::AccessFlagBits::eShaderRead;
				barrier.dstAccessMask = vk::AccessFlagBits::eDepthStencilAttachmentRead | vk::AccessFlagBits::eDepthStencilAttachmentWrite;
				_setImageMemoryBarrierVK(barrier, vk::PipelineStageFlagBits::eFragmentShader, vk::PipelineStageFlagBits::eEarlyFragmentTests);
				return;
			}
		}
		ZE_FAIL("CommandBufferVulkan: Image transition layout is not supported!");
	}


	void CommandBufferVulkan::CopyBufferToTextureVK(BufferVulkan& source, TextureVulkan& destination)
	{
		ZE_ASSERT(source.GetUsageType() == BufferUsageType::Staging, "CommandBufferVulkan: Invalid source buffer usage type for copying to a texture!");
		const Vec2i& size = destination.GetExtent();
		const vk::ImageSubresourceLayers subresource(vk::ImageAspectFlagBits::eColor, 0, 0, 1);
		const vk::Extent3D extent(size.X, size.Y, 1);
		const vk::BufferImageCopy bufferImageCopy(0, 0, 0, subresource, vk::Offset3D(), extent);
		_getCurrent().copyBufferToImage(source.GetBufferVK(), destination.GetImageVK(), vk::ImageLayout::eTransferDstOptimal, 1, &bufferImageCopy);
	}

	vk::CommandBuffer& CommandBufferVulkan::_getCurrent()
	{
		ZE_ASSERT(mCommandBufferList.size() > mCurrentCommandBufferIndex);
		return *mCommandBufferList[mCurrentCommandBufferIndex];
	}
	const vk::CommandBuffer& CommandBufferVulkan::_getCurrent() const
	{
		ZE_ASSERT(mCommandBufferList.size() > mCurrentCommandBufferIndex);
		return *mCommandBufferList[mCurrentCommandBufferIndex];
	}

	void CommandBufferVulkan::_acquireNextBuffer()
	{
		mCurrentCommandBufferIndex = (mCurrentCommandBufferIndex + 1) % mCommandBufferList.size();
	}

	void CommandBufferVulkan::_setImageMemoryBarrierVK(const vk::ImageMemoryBarrier& barrier, vk::PipelineStageFlagBits src, vk::PipelineStageFlagBits dst)
	{
		_getCurrent().pipelineBarrier(src, dst, vk::DependencyFlags(), 0, nullptr, 0, nullptr, 1, &barrier);
	}
}

#endif