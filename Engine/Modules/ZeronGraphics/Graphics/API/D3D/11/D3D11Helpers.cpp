// Copyright (C) Eser Kokturk. All Rights Reserved.

#if ZE_GRAPHICS_D3D
#include <Graphics/API/D3D/11/D3D11Helpers.h>

#include <d3d11.h>
#include <Graphics/API/D3D/DebugInfoD3D.h>

namespace Zeron
{
	D3D11_BIND_FLAG D3D11Helpers::GetBufferType(BufferType bufferType)
	{
		switch (bufferType) {
			case BufferType::Vertex: return D3D11_BIND_FLAG::D3D11_BIND_VERTEX_BUFFER;
			case BufferType::Index: return D3D11_BIND_FLAG::D3D11_BIND_INDEX_BUFFER;
			case BufferType::Uniform: return D3D11_BIND_FLAG::D3D11_BIND_CONSTANT_BUFFER;
			case BufferType::Undefined: return static_cast<D3D11_BIND_FLAG>(0);
			default: ZE_FAIL("D3D11 buffer type is not supported!");
		}
		return static_cast<D3D11_BIND_FLAG>(0);
	}

	D3D11_CULL_MODE D3D11Helpers::GetCullMode(FaceCullMode cullMode)
	{
		switch (cullMode) {
		case FaceCullMode::None: return D3D11_CULL_MODE::D3D11_CULL_NONE;
			case FaceCullMode::Back: return D3D11_CULL_MODE::D3D11_CULL_BACK;
			case FaceCullMode::Front: return D3D11_CULL_MODE::D3D11_CULL_FRONT;
			case FaceCullMode::FrontAndBack:;
			default: ZE_FAIL("D3D11 cull mode is not supported");
		}
	}

	UINT D3D11Helpers::GetMultiSampleLevel(MSAALevel level)
	{
		switch (level) {
			case MSAALevel::Disabled: return 1;
			case MSAALevel::x2: return 2;
			case MSAALevel::x4: return 4;
			case MSAALevel::x8: return 8;
			default: ZE_FAIL("D3D11 multi-sample anti aliasing level is not supported!");
		}
		return 0;
	}
}
#endif
