// Copyright (C) Eser Kokturk. All Rights Reserved.

#include <Script/JS/V8/ScriptRuntimeV8.h>

#include <libplatform/libplatform.h>
#include <Script/JS/V8/ScriptContextV8.h>
#include <Script/ScriptRuntimeType.h>
#include <Script/Templates/ScriptGlobalTemplate.h>


namespace Zeron::Script
{
	RuntimeV8::RuntimeV8(v8::Platform& platform)
		: mPlatform(platform)
		, mIsolate(nullptr)
		, mContextCounter(0)
		, mActiveContext(nullptr)
	{
		v8::Isolate::CreateParams create_params;
		create_params.array_buffer_allocator = v8::ArrayBuffer::Allocator::NewDefaultAllocator();
		mIsolate = v8::Isolate::New(create_params);
	}

	RuntimeV8::~RuntimeV8()
	{
		ForceGarbageCollection();

		ExitScope();
		mContexts.clear();
		mIsolate->Dispose();
	}


	Context* RuntimeV8::CreateContext(GlobalTemplate globalTemplate)
	{
		EnterScope(nullptr);
		v8::HandleScope localScope = HandleLocalScopeV8();
		const auto result = mContexts.emplace(mContextCounter, std::make_unique<ContextV8>(NotNull(this), mContextCounter, globalTemplate));
		++mContextCounter;
		return result.first->second.get();
	}

	Context* RuntimeV8::GetContext(size_t contextId)
	{
		const auto context = mContexts.find(contextId);
		return context != mContexts.end() ? context->second.get() : nullptr;
	}

	bool RuntimeV8::RemoveContext(Context* context)
	{
		return false;
	}

	void RuntimeV8::EnterScope(Context* context)
	{
		if (!mScope.has_value()) {
			mScope.emplace(mIsolate);
		}
		if (context && context != mActiveContext) {
			if (const auto found = mContexts.find(context->GetId()); found != mContexts.end() && context == found->second.get()) {
				ActivateContextV8(*static_cast<ContextV8*>(context));
			}
			else {
				ZE_FAIL("Expected to find script context for runtime.");
			}
		}
	}

	void RuntimeV8::ExitScope()
	{
		if (mActiveContext) {
			mActiveContext->ExitScopeV8();
		}
		mScope.reset();
	}

	bool RuntimeV8::IsInScope(Context* context) const
	{
		return mScope.has_value() && (context != nullptr ? context == mActiveContext : true);
	}

	RuntimeType RuntimeV8::GetType() const
	{
		return RuntimeType::JavaScriptV8;
	}

	void RuntimeV8::ForceGarbageCollection()
	{
		mIsolate->LowMemoryNotification();
	}

	v8::HandleScope RuntimeV8::HandleLocalScopeV8() const
	{
		return v8::HandleScope(mIsolate);
	}

	void RuntimeV8::ActivateContextV8(ContextV8& context)
	{
		if (mActiveContext) {
			mActiveContext->ExitScopeV8();
		}
		mActiveContext = &context;
		mActiveContext->EnterScopeV8();
	}

}