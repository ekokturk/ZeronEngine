// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

#include <Script/ScriptContext.h>
#include <Script/Templates/ScriptGlobalTemplate.h>
#include <v8-context.h>
#include <v8-persistent-handle.h>

namespace Zeron::Script
{
	class RuntimeV8;
	class ObjectManagerV8;

	class ContextV8 final : public Context {
	  public:
		ContextV8(NotNull<RuntimeV8> runtime, size_t id, GlobalTemplate global = {});
		~ContextV8() override;

		size_t GetId() override;
		Value Run(std::string_view script) override;

		// V8
		ObjectManagerV8* GetObjectManager() const;
		v8::Local<v8::Context> GetContextV8() const;
		v8::Isolate* GetIsolateV8() const;
		void EnterScopeV8();
		void ExitScopeV8();

	  private:
		NotNull<RuntimeV8> mRuntime;
		size_t mId;

		v8::Global<v8::Context> mContext;

		// Stores native bound object instances
		std::unique_ptr<ObjectManagerV8> mObjectManager;

		// Native bindings for global object
		GlobalTemplate mNativeGlobalTemplate;
	};

}