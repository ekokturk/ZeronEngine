// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

namespace Zeron
{
	class GraphicsAdapter {
	  public:
		virtual ~GraphicsAdapter() = default;

		virtual const char* GetName() const = 0;
		virtual bool HasSwapChainSupport() const = 0;
		virtual bool IsDiscreteAdapter() const = 0;
	};
}