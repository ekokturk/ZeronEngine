// Copyright (C) Eser Kokturk. All Rights Reserved.

#include <Script/JS/V8/ScriptObjectManagerV8.h>

#include <Script/JS/V8/ScriptContextV8.h>
#include <Script/JS/V8/ScriptObjectStorageV8.h>
#include <Script/JS/V8/ScriptUtilsV8.h>
#include <Script/Templates/ScriptObjectTemplate.h>
#include <v8-function.h>

namespace Zeron::Script
{

	ObjectManagerV8::ObjectManagerV8(ContextV8* ctx)
		: mContext(ctx)
	{}

	ObjectManagerV8::~ObjectManagerV8() {}

	v8::Local<v8::FunctionTemplate> ObjectManagerV8::Register(const ObjectTemplate& objTemplate)
	{
		v8::EscapableHandleScope handleScope(mContext->GetIsolateV8());

		const size_t id = objTemplate.mTypeInfo.GetId();
		ZE_ASSERT(!mStorage.contains(id), "Object type is already registered.");

		std::unique_ptr<ObjectStorageV8> storage = std::make_unique<ObjectStorageV8>(mContext, this, objTemplate);
		const v8::Local<v8::FunctionTemplate> objTemplateV8 = handleScope.Escape(storage->mJSTemplate.Get(mContext->GetIsolateV8()));
		mStorage.emplace(id, std::move(storage));

		return objTemplateV8;
	}

	ObjectStorageV8* ObjectManagerV8::GetStorageV8(const size_t& typeId)
	{
		return static_cast<ObjectStorageV8*>(GetStorage(typeId));
	}
}