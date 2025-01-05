// Copyright (C) Eser Kokturk. All Rights Reserved.

#ifdef ZE_GRAPHICS_D3D

#	include <Graphics/API/D3D/11/CommandBufferD3D11.h>

#	include <d3d11_1.h>
#	include <Graphics/API/D3D/11/BufferD3D11.h>
#	include <Graphics/API/D3D/11/FrameBufferD3D11.h>
#	include <Graphics/API/D3D/11/GraphicsD3D11.h>
#	include <Graphics/API/D3D/11/PipelineBindingD3D11.h>
#	include <Graphics/API/D3D/11/PipelineD3D11.h>
#	include <Graphics/API/D3D/11/ShaderD3D11.h>
#	include <Graphics/API/D3D/DebugInfoD3D.h>

namespace Zeron::Gfx
{
	CommandBufferD3D11::CommandBufferD3D11(GraphicsD3D11& graphics)
		: mDeviceContext(graphics.GetDeviceContextD3D())
		, mClearColor(Color::Black)
		, mFrameBuffer(nullptr)
		, mPipeline(nullptr)
		, mPipelineBinding(nullptr)
	{
#	if ZE_DEBUG
		mDeviceContext->QueryInterface(__uuidof(ID3DUserDefinedAnnotation), reinterpret_cast<void**>(mDebugAnnotation.GetAddressOf()));
#	endif
	}

	void CommandBufferD3D11::Begin() {}

	void CommandBufferD3D11::BeginRenderPass(FrameBuffer* frameBuffer)
	{
		ZE_ASSERT(frameBuffer, "Frame buffer is not available for D3D11 render pass!");
		ZE_ASSERT(!mFrameBuffer && !mPipeline && !mPipelineBinding, "Expected render pass state to be empty");
		mFrameBuffer = static_cast<FrameBufferD3D11*>(frameBuffer);
		mFrameBuffer->BindD3D(mDeviceContext, mClearColor);
	}

	void CommandBufferD3D11::EndRenderPass()
	{
		_clearResources();
	}

	void CommandBufferD3D11::End() {}

	void CommandBufferD3D11::Clear(Color color)
	{
		mClearColor = color;
	}

	void CommandBufferD3D11::SetScissor(const Vec2i& extent, const Vec2i& offset)
	{
		const D3D11_RECT rect = { offset.X, offset.Y, extent.X, extent.Y };
		ZE_D3D_ASSERT(mDeviceContext->RSSetScissorRects(1, &rect));
	}

	void CommandBufferD3D11::SetViewport(const Vec2i& size, const Vec2i& position)
	{
		D3D11_VIEWPORT viewport;
		ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));
		viewport.TopLeftX = static_cast<FLOAT>(position.X);
		viewport.TopLeftY = static_cast<FLOAT>(position.Y);
		viewport.Width = static_cast<FLOAT>(size.X);
		viewport.Height = static_cast<FLOAT>(size.Y);
		viewport.MinDepth = 0.f;
		viewport.MaxDepth = 1.f;

		ZE_D3D_ASSERT(mDeviceContext->RSSetViewports(1, &viewport));
	}

	void CommandBufferD3D11::SetPipeline(Pipeline& pipeline)
	{
		if (mPipeline && mPipeline != &pipeline) {
			_clearPipeline();
		}
		BeginDebugGroup("Pipeline Update");
		mPipeline = static_cast<PipelineD3D11*>(&pipeline);
		mPipeline->Apply(mDeviceContext);
		EndDebugGroup();
	}

	void CommandBufferD3D11::SetPipelineBinding(PipelineBinding& binding, uint32_t index)
	{
		if (mPipelineBinding && mPipelineBinding != &binding) {
			_clearPipelineBinding();
		}
		BeginDebugGroup("Pipeline Binding Update");
		mPipelineBinding = static_cast<PipelineBindingD3D11*>(&binding);
		// TODO: Support sets so index can be used
		mPipelineBinding->Apply(mDeviceContext);
		EndDebugGroup();
	}

	void CommandBufferD3D11::SetVertexBuffer(Buffer& vb, uint32_t slot)
	{
		ZE_ASSERT(vb.GetBufferType() == BufferType::Vertex, "Invalid buffer type!");
		const auto& bufferDx = static_cast<BufferD3D11&>(vb);
		ID3D11Buffer* arr[] = { bufferDx.GetBufferD3D() };
		const uint32_t stride[] = { bufferDx.GetStride() };
		const uint32_t offset[] = { 0 };
		ZE_D3D_ASSERT(mDeviceContext->IASetVertexBuffers(slot, 1, arr, stride, offset));
	}

	void CommandBufferD3D11::SetVertexBuffers(Buffer** vb, uint32_t count, uint32_t slot)
	{
		std::vector<ID3D11Buffer*> arr(count);
		std::vector<uint32_t> stride(count);
		std::vector<uint32_t> offset(count);
		for (uint32_t i = 0; i < count; ++i) {
			const BufferD3D11* bufferDx = static_cast<BufferD3D11*>(vb[i]);
			ZE_ASSERT(bufferDx->GetBufferType() == BufferType::Vertex, "Invalid buffer type!");
			arr[i] = bufferDx->GetBufferD3D();
			stride[i] = bufferDx->GetStride();
			offset[i] = 0;
		}
		ZE_D3D_ASSERT(mDeviceContext->IASetVertexBuffers(slot, count, arr.data(), stride.data(), offset.data()));
	}

	void CommandBufferD3D11::SetIndexBuffer(Buffer& ib)
	{
		ZE_ASSERT(ib.GetBufferType() == BufferType::Index, "Invalid D3D11  buffer type!");
		ZE_ASSERT(ib.GetStride() == 2 || ib.GetStride() == 4, "Unsupported D3D11 index buffer stride!");
		const auto& bufferDx = static_cast<BufferD3D11&>(ib);
		ZE_D3D_ASSERT(mDeviceContext->IASetIndexBuffer(bufferDx.GetBufferD3D(), ib.GetStride() == 2 ? DXGI_FORMAT::DXGI_FORMAT_R16_UINT : DXGI_FORMAT::DXGI_FORMAT_R32_UINT, 0)
		);
	}

	void CommandBufferD3D11::CopyBuffer(Buffer& source, Buffer& destination)
	{
		ZE_FAIL("Not Implemented!");
	}

	void CommandBufferD3D11::UpdateBuffer(Buffer& buff, const void* data, uint32_t sizeBytes, uint32_t offset, BufferUpdateRule updateRule)
	{
		ZE_ASSERT(
			(buff.GetCount() - offset) * buff.GetStride() >= sizeBytes, "D3D11 buffer size {} does not match for update data size {}!", buff.GetSizeInBytes(), sizeBytes
		);
		auto& buffDx = static_cast<BufferD3D11&>(buff);
		buffDx.UpdateD3D(mDeviceContext, data, sizeBytes, offset, updateRule);
	}

	void CommandBufferD3D11::SetPushConstant(const void* data, uint32_t stride, ShaderType shaderType)
	{
		ZE_ASSERT(mPipeline, "CommandBufferD3D11: Expected Pipeline to be bound to update push constant");
		mPipeline->SetPushConstantBuffer(mDeviceContext, data, stride, shaderType);
	}

	void CommandBufferD3D11::Draw(uint32_t vertexCount, uint32_t vertexStart)
	{
		ZE_D3D_ASSERT(mDeviceContext->Draw(vertexCount, vertexStart));
	}

	void CommandBufferD3D11::DrawIndexed(uint32_t indexCount, uint32_t indexStart, uint32_t vertexStart)
	{
		ZE_D3D_ASSERT(mDeviceContext->DrawIndexed(indexCount, indexStart, vertexStart));
	}

	void CommandBufferD3D11::DrawInstanced(uint32_t vertexCount, uint32_t instanceCount, uint32_t vertexStart, uint32_t instanceStart)
	{
		ZE_D3D_ASSERT(mDeviceContext->DrawInstanced(vertexCount, instanceCount, vertexStart, instanceStart));
	}

	void CommandBufferD3D11::DrawInstancedIndexed(uint32_t indexCount, uint32_t instanceCount, uint32_t indexStart, uint32_t baseVertexLocation, uint32_t instanceStart)
	{
		// TODO: Add Debug assert here to warn if instanceStart is being used without instanced input
		ZE_D3D_ASSERT(mDeviceContext->DrawIndexedInstanced(indexCount, instanceCount, indexStart, baseVertexLocation, instanceStart));
	}

	void CommandBufferD3D11::Dispatch(uint32_t countX, uint32_t countY, uint32_t countZ)
	{
		ZE_D3D_ASSERT(mDeviceContext->Dispatch(countX, countY, countZ));
		// TODO: Make this more explicit, maybe as a separate begin/end pass or as an argument
		_clearResources();
	}

	void CommandBufferD3D11::AddBarrier(Texture& texture, TextureLayout oldLayout, TextureLayout newLayout) {}

	void CommandBufferD3D11::BeginDebugGroup(std::string_view label) const
	{
#	if ZE_DEBUG
		if (mDebugAnnotation) {
			const std::wstring evtLabel(label.begin(), label.end());
			mDebugAnnotation->BeginEvent(evtLabel.c_str());
		}
#	endif
	}

	void CommandBufferD3D11::EndDebugGroup() const
	{
#	if ZE_DEBUG
		if (mDebugAnnotation) {
			mDebugAnnotation->EndEvent();
		}
#	endif
	}

	uint32_t CommandBufferD3D11::GetBufferCount() const
	{
		return 1;
	}

	void CommandBufferD3D11::_clearResources()
	{
		if (mPipelineBinding) {
			_clearPipelineBinding();
		}
		if (mPipeline) {
			_clearPipeline();
		}
		if (mFrameBuffer) {
			mFrameBuffer->UnbindD3D(mDeviceContext);
			mFrameBuffer = nullptr;
		}
	}

	void CommandBufferD3D11::_clearPipeline()
	{
		BeginDebugGroup("Pipeline Cleanup");
		mPipeline->Clear(mDeviceContext);
		mPipeline = nullptr;
		EndDebugGroup();
	}

	void CommandBufferD3D11::_clearPipelineBinding()
	{
		BeginDebugGroup("Pipeline Binding Cleanup");
		mPipelineBinding->Clear(mDeviceContext);
		mPipelineBinding = nullptr;
		EndDebugGroup();
	}
}
#endif