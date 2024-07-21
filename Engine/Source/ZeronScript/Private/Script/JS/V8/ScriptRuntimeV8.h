// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

#include <Script/ScriptRuntime.h>
#include <v8-isolate.h>
#include <v8-local-handle.h>

namespace v8
{
	class Platform;
	class Isolate;
}

namespace Zeron::Script
{
	class ContextV8;

	class RuntimeV8 final : public Runtime {
	  public:
		RuntimeV8(v8::Platform& platform);
		~RuntimeV8() override;

		// Runtime
		Context* CreateContext(GlobalTemplate globalTemplate) override;
		Context* GetContext(size_t contextId);
		bool RemoveContext(Context* context) override;
		RuntimeType GetType() const override;

		void EnterScope(Context* context) override;
		void ExitScope() override;
		bool IsInScope(Context* context) const override;

		void ForceGarbageCollection() override;

		// V8
		v8::Isolate* GetIsolateV8() const { return mIsolate; }
		[[nodiscard]] v8::HandleScope HandleLocalScopeV8() const;
		void ActivateContextV8(ContextV8& context);

	  private:
		v8::Platform& mPlatform;
		v8::Isolate* mIsolate;
		std::optional<v8::Isolate::Scope> mScope;
		size_t mContextCounter;
		std::unordered_map<size_t, std::unique_ptr<ContextV8>> mContexts;
		ContextV8* mActiveContext;
	};

}