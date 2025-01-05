// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

namespace Sandbox
{
	class ISandboxInstance {
	  public:
		virtual ~ISandboxInstance() = default;
		virtual void Init() = 0;
		virtual void Update() {}
		virtual void Tick() {}

		bool IsInit() const { return mIsInit; }
		bool IsRunning() const { return mIsRunning; }

	  protected:
		bool mIsInit = false;
		bool mIsRunning = false;
	};

}