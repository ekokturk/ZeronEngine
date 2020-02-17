// Copyright (C) 2020, Eser Kokturk. All Rights Reserved.

#include "Core.h"

#pragma once

namespace ZeronEngine
{
	class ZRN_API Application
	{
	private:

	public:
		Application();
		virtual ~Application();

		virtual void Run();
		virtual void Destroy();
	};

}
