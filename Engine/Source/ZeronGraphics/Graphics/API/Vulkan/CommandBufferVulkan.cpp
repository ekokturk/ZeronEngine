// Copyright (C) Eser Kokturk. All Rights Reserved.

#if ZE_GRAPHICS_VULKAN
#include <Graphics/API/Vulkan/CommandBufferVulkan.h>

#include <Graphics/API/Vulkan/BufferVulkan.h>
#include <Graphics/API/Vulkan/FrameBufferVulkan.h>
#include <Graphics/API/Vulkan/GraphicsVulkan.h>
#include <Graphics/API/Vulkan/PipelineVulkan.h>
#include <Graphics/API/Vulkan/PipelineBindingVulkan.h>
#include <Graphics/API/Vulkan/RenderPassVulkan.h>
#include <Graphics/API/Vulkan/TextureVulkan.h>

namespace Zeron
{
	CommandBufferVulkan::CommandBufferVulkan(GraphicsVulkan& graphics, uint32_t count, bool isCompute)
		: mDevice(graphics.GetDeviceVK())
		, mIsCompute(isCompute)
		, mCommandPool(graphics.CreateCommandPoolVK(mIsCompute))
		, mCurrentCommandBufferIndex(0)
		, mCurrentRenderPass(nullptr)
		, mCurrentFrameBuffer(nullptr)
		, mCurrentPipeline(nullptr)
	{
		const vk::CommandBufferAllocateInfo allocateInfo(
			*mCommandPool,
			vk::CommandBufferLevel::ePrimary,
			count
		);
		mCommandBufferList = graphics.GetDeviceVK().allocateCommandBuffersUnique(allocateInfo);
	}

	void CommandBufferVulkan::Begin()
	{
		_acquireNextBuffer();
		const vk::CommandBufferBeginInfo beginInfo {
			vk::CommandBufferUsageFlagBits::eOneTimeSubmit,
			nullptr
		};
		_getCurrent().reset(vk::CommandBufferResetFlagBits::eReleaseResources);
		_getCurrent().begin(beginInfo);
	}

	void CommandBufferVulkan::BeginRenderPass(FrameBuffer* frameBuffer, RenderPass* renderPass)
	{
		mCurrentFrameBuffer = static_cast<FrameBufferVulkan*>(frameBuffer);
		mCurrentRenderPass = static_cast<RenderPassVulkan*>(renderPass);
		ZE_ASSERT(mCurrentRenderPass, "Vulkan command buffer cannot have null render pass!");
		ZE_ASSERT(mCurrentFrameBuffer, "Vulkan command buffer cannot have null frame buffer!");

		const vk::Extent2D& extent = mCurrentFrameBuffer->GetExtentVK();
		ZE_ASSERT(mScissor.extent.width <= extent.width && mScissor.extent.height <= extent.height, "Vulkan render pass has to be within frame buffer extent!");

		const vk::RenderPassBeginInfo beginInfo(
			mCurrentRenderPass->GetRenderPassVK(),
			mCurrentFrameBuffer->GetFrameBufferVK(),
			mScissor,
			static_cast<uint32_t>(mClearValue.size()),
			mClearValue.data()
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
			vk::ClearDepthStencilValue { 1.0f, 0 },
			vk::ClearColorValue(std::array<float,4>{ color.normR(), color.normG(), color.normB(), color.normA() }),
		};
	}

	void CommandBufferVulkan::SetScissor(const Vec2i& extent, const Vec2i& offset)
	{
		mScissor = vk::Rect2D(
			{ offset.X, offset.Y },
			{ static_cast<uint32_t>(extent.X), static_cast<uint32_t>(extent.Y) }
		);
		_getCurrent().setScissor(0, 1, &mScissor);
	}

	void CommandBufferVulkan::SetViewport(const Vec2i& size, const Vec2i& position)
	{
		// Flipping viewport to correct axises
		const vk::Viewport info(
			static_cast<float>(position.X),
			static_cast<float>(position.Y + size.Y),
			static_cast<float>(size.X),
			static_cast<float>(-size.Y),
			0.0f,
			1.0f
		);
		_getCurrent().setViewport(0, 1, &info);
	}

	void CommandBufferVulkan::SetPipeline(Pipeline& pipeline)
	{
		mCurrentPipeline = static_cast<PipelineVulkan*>(&pipeline);
		_getCurrent().bindPipeline(mIsCompute ? vk::PipelineBindPoint::eCompute : vk::PipelineBindPoint::eGraphics, mCurrentPipeline->GetPipelineVK());
	}

	void CommandBufferVulkan::SetPipelineBinding(PipelineBinding& binding, uint32_t index)
	{
		ZE_ASSERT(mCurrentPipeline, "Cannot set Vulkan pipeline layout without an existing pipeline!");
		const auto& bindingVk = static_cast<PipelineBindingVulkan&>(binding);
		const auto& descriptorSets = bindingVk.GetDescriptorSets();
		_getCurrent().bindDescriptorSets(
			mIsCompute ? vk::PipelineBindPoint::eCompute : vk::PipelineBindPoint::eGraphics,
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
		ZE_ASSERT(vb.GetBufferType() == BufferType::Vertex, "Vulkan command expected vertex buffer!");
		auto& bufferVk = static_cast<BufferVulkan&>(vb);
		_getCurrent().bindVertexBuffers(slot, bufferVk.GetBufferVK(), { 0 });
	}

	void CommandBufferVulkan::SetVertexBuffers(Buffer** vb, uint32_t count, uint32_t slot)
	{
		std::vector<vk::Buffer> buffers(count);
		std::vector <vk::DeviceSize> offsets(count);
		for (uint32_t i = 0; i < count; ++i) {
			auto* bufferVk = static_cast<BufferVulkan*>(vb[i]);
			ZE_ASSERT(bufferVk->GetBufferType() == BufferType::Vertex, "Vulkan command expected vertex buffer!");
			buffers[i] = bufferVk->GetBufferVK();
			offsets[i] = 0;
		}
		_getCurrent().bindVertexBuffers(slot, count, buffers.data(), offsets.data());
	}

	void CommandBufferVulkan::SetIndexBuffer(Buffer& ib)
	{
		ZE_ASSERT(ib.GetBufferType() == BufferType::Index, "Vulkan command expected index buffer!");
		ZE_ASSERT(ib.GetStride() == 2 || ib.GetStride() == 4, "Unsupported Vulkan index buffer stride!");
		auto& bufferVk = static_cast<BufferVulkan&>(ib);
		_getCurrent().bindIndexBuffer(bufferVk.GetBufferVK(), 0, ib.GetStride() == 2 ? vk::IndexType::eUint16 : vk::IndexType::eUint32);
	}

	void CommandBufferVulkan::CopyBuffer(Buffer& source, Buffer& destination)
	{
		ZE_ASSERT(source.GetSizeInBytes() == destination.GetSizeInBytes(), "Vulkan buffer sizes do not match for copy command!");
		auto& sourceVk = static_cast<BufferVulkan&>(source);
		auto& destinationVk = static_cast<BufferVulkan&>(destination);
		const vk::BufferCopy copyRegion(0, 0, source.GetSizeInBytes());
		_getCurrent().copyBuffer(sourceVk.GetBufferVK(), destinationVk.GetBufferVK(), copyRegion);
	}

	void CommandBufferVulkan::UpdateBuffer(Buffer& buff, void* data, uint32_t sizeBytes, uint32_t offset, BufferUpdateRule updateRule)
	{
		auto& bufferVk = static_cast<BufferVulkan&>(buff);
		bufferVk.UpdateVK(mDevice, data, sizeBytes, offset, updateRule);
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
		ZE_ASSERT(mIsCompute, "Vulkan command buffer is not compute!");
		_getCurrent().dispatch(countX, countY, countZ);
	}

	uint32_t CommandBufferVulkan::GetBufferCount() const
	{
		return static_cast<uint32_t>(mCommandBufferList.size());
	}

	vk::CommandBuffer& CommandBufferVulkan::GetCommandBufferVK()
	{
		return _getCurrent();
	}

	bool CommandBufferVulkan::IsCompute() const
	{
		return mIsCompute;
	}

	void CommandBufferVulkan::ApplyImageTransitionLayoutVK(TextureVulkan& texture, vk::ImageLayout oldLayout, vk::ImageLayout newLayout)
	{
		vk::ImageMemoryBarrier barrier;
		barrier.image = texture.GetImageVK();
		barrier.oldLayout = oldLayout;
		barrier.newLayout = newLayout;
		barrier.subresourceRange.aspectMask = vk::ImageAspectFlagBits::eColor;
		barrier.subresourceRange.baseMipLevel = 0;
		barrier.subresourceRange.levelCount = texture.GetMipLevelVK();
		barrier.subresourceRange.baseArrayLayer = 0;
		barrier.subresourceRange.layerCount = 1;

		if(oldLayout == vk::ImageLayout::eUndefined) {
			if (newLayout == vk::ImageLayout::eColorAttachmentOptimal) {
				barrier.dstAccessMask = vk::AccessFlagBits::eColorAttachmentRead | vk::AccessFlagBits::eColorAttachmentWrite;
				_setImageMemoryBarrierVK(barrier, vk::PipelineStageFlagBits::eTopOfPipe, vk::PipelineStageFlagBits::eColorAttachmentOutput);
				return;
			}
			if (newLayout == vk::ImageLayout::eDepthStencilAttachmentOptimal) {
				barrier.dstAccessMask = vk::AccessFlagBits::eDepthStencilAttachmentRead | vk::AccessFlagBits::eDepthStencilAttachmentWrite;
				barrier.subresourceRange.aspectMask = vk::ImageAspectFlagBits::eDepth;
				_setImageMemoryBarrierVK(barrier, vk::PipelineStageFlagBits::eTopOfPipe, vk::PipelineStageFlagBits::eEarlyFragmentTests);
				return;
			}
			if (newLayout == vk::ImageLayout::eTransferDstOptimal) {
				barrier.dstAccessMask = vk::AccessFlagBits::eTransferWrite;
				_setImageMemoryBarrierVK(barrier, vk::PipelineStageFlagBits::eTopOfPipe, vk::PipelineStageFlagBits::eTransfer);
				return;
			}
		}
		else if (oldLayout == vk::ImageLayout::eTransferDstOptimal) {
			if (newLayout == vk::ImageLayout::eShaderReadOnlyOptimal) {
				barrier.srcAccessMask = vk::AccessFlagBits::eTransferWrite;
				barrier.dstAccessMask = vk::AccessFlagBits::eShaderRead;
				_setImageMemoryBarrierVK(barrier, vk::PipelineStageFlagBits::eTransfer, vk::PipelineStageFlagBits::eFragmentShader);
				return;
			}
		}
		ZE_FAIL("Vulkan image transition layout is not supported!");
	}


	void CommandBufferVulkan::CopyBufferToTextureVK(BufferVulkan& source, TextureVulkan& destination)
	{
		ZE_ASSERT(source.GetUsageType() == BufferUsageType::Staging, "Invalid Vulkan source buffer usage type for copying to a texture!");
		const Vec2i& size = destination.GetSize();
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

