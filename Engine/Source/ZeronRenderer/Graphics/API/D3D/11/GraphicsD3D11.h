// Copyright (C) 2020, Eser Kokturk. All Rights Reserved.

#pragma once

#include "Graphics/API/D3D/GraphicsD3D.h"
#if ZE_GRAPHICS_D3D
#include "Graphics/API/D3D/11/GraphicsContextD3D11.h"
#include <d3d11.h>

struct ID3D11Device;
struct ID3D11DeviceContext;
#endif

namespace Zeron {
	 
	class GraphicsContextD3D11;

	class GraphicsD3D11 final : public GraphicsD3D
	{
	public:
		virtual bool Init() override;
		virtual bool Destroy() override;
		virtual void RenderFrame() override;

		virtual void CreateGraphicsContext(Window* window) override;

#if ZE_GRAPHICS_D3D
	public:
		ID3D11Device* GetDevice() const;
		ID3D11DeviceContext* GetDeviceContext() const;

	private:
		Microsoft::WRL::ComPtr<ID3D11Device> mDevice;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> mDeviceContext;

		std::vector<std::unique_ptr<GraphicsContextD3D11>> mGraphicsContexes;
#endif
	};
}
