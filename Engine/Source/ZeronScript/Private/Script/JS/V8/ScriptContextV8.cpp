// Copyright (C) Eser Kokturk. All Rights Reserved.

#include <Script/JS/V8/ScriptContextV8.h>

#include <Script/JS/V8/ScriptObjectManagerV8.h>
#include <Script/JS/V8/ScriptObjectStorageV8.h>
#include <Script/JS/V8/ScriptRuntimeV8.h>
#include <Script/JS/V8/ScriptUtilsV8.h>
#include <Script/Templates/ScriptGlobalTemplate.h>
#include <v8-external.h>
#include <v8-function.h>
#include <v8-template.h>

namespace Zeron::Script
{

	ContextV8::ContextV8(NotNull<RuntimeV8> runtime, size_t id, GlobalTemplate global)
		: mRuntime(runtime)
		, mId(id)
		, mObjectManager(std::make_unique<ObjectManagerV8>(this))
		, mNativeGlobalTemplate(std::move(global))
	{
		// Initialize global template from native template
		const v8::Local<v8::ObjectTemplate> globalTemplate = v8::ObjectTemplate::New(GetIsolateV8());
		for (const auto& moduleClass : mNativeGlobalTemplate.mClasses) {
			// We register a new storage for each class
			const v8::Local<v8::FunctionTemplate> classTemplate = mObjectManager->Register(moduleClass.second);
			globalTemplate->Set(UtilsV8::FromString(GetIsolateV8(), moduleClass.first), classTemplate);
		}
		for (auto& [name, function] : mNativeGlobalTemplate.mFunctions) {
			const v8::Local<v8::FunctionTemplate> funcTemp = v8::FunctionTemplate::New(
				GetIsolateV8(), UtilsV8::CreateFunctionCallback(), v8::External::New(GetIsolateV8(), &function)
			);
			globalTemplate->Set(UtilsV8::FromString(GetIsolateV8(), name), funcTemp);
		}

		// Create context
		v8::Local<v8::Context> context = v8::Context::New(GetIsolateV8(), nullptr, globalTemplate);
		v8::Global<v8::Context> persistentContext(GetIsolateV8(), context);
		mContext = std::move(persistentContext);

		// Activate context and initialize global object instances
		mRuntime->ActivateContextV8(*this);
		const v8::Local<v8::Object> globalInstance = context->Global();
		for (auto& [name, factory] : mNativeGlobalTemplate.mInstances) {
			const auto instance = factory.mCreateFn(*mObjectManager);
			globalInstance->Set(context, UtilsV8::FromString(GetIsolateV8(), name), mObjectManager->GetStorageV8(factory.mTypeInfo.GetId())->GetInstanceV8(instance.mHandle));
		}
	}

	ContextV8::~ContextV8()
	{
		mContext.Reset();
	}

	size_t ContextV8::GetId()
	{
		return mId;
	}

	void ContextV8::EnterScopeV8()
	{
		v8::HandleScope scope = mRuntime->HandleLocalScopeV8();
		mContext.Get(mRuntime->GetIsolateV8())->Enter();
	}

	void ContextV8::ExitScopeV8()
	{
		v8::HandleScope scope = mRuntime->HandleLocalScopeV8();
		mContext.Get(mRuntime->GetIsolateV8())->Exit();
	}

	Value ContextV8::Run(std::string_view script)
	{
		ZE_ASSERT(mRuntime->IsInScope(this), "Expected owning runtime to be in scope!");
		v8::HandleScope scope = mRuntime->HandleLocalScopeV8();
		v8::Local<v8::Context> context = mContext.Get(GetIsolateV8());

		if (!script.empty()) {
			v8::Local<v8::Script> compiledScript;
			if (v8::Script::Compile(context, UtilsV8::FromString(GetIsolateV8(), script)).ToLocal(&compiledScript)) {
				v8::Local<v8::Value> result;
				compiledScript->Run(context).ToLocal(&result);
				return UtilsV8::ToValue(GetIsolateV8(), result);
			}
		}
		// TODO: Maybe error here
		return {};
	}

	v8::Local<v8::Context> ContextV8::GetContextV8() const
	{
		return mContext.Get(GetIsolateV8());
	}

	v8::Isolate* ContextV8::GetIsolateV8() const
	{
		return mRuntime->GetIsolateV8();
	}

	ObjectManagerV8* ContextV8::GetObjectManager() const
	{
		return mObjectManager.get();
	}
}