// Copyright (C) 2020, Eser Kokturk. All Rights Reserved.

#pragma once
#include "Graphics/Graphics.h"
#include "GraphicsAdapterD3D.h"
#if ZE_GRAPHICS_D3D
#include <wrl.h>

#endif

namespace Zeron {
	
	class GraphicsD3D : public Graphics
	{
	public:

		virtual ~GraphicsD3D();

		
		static std::vector<GraphicsAdapterD3D>& GetAdapters();
	
	private:
		static std::vector<GraphicsAdapterD3D> mGraphicsAdapters;
	};

}
