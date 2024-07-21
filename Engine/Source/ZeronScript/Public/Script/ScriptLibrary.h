// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

namespace Zeron::Script
{
	class Runtime;

	class Library {
	  public:
		virtual ~Library() = default;

		virtual std::unique_ptr<Runtime> CreateRuntime() = 0;
	};
}