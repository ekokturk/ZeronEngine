// Copyright (C) Eser Kokturk. All Rights Reserved.

#include <Script/JS/V8/ScriptObjectStorageV8.h>

#include <Script/JS/V8/ScriptContextV8.h>
#include <Script/JS/V8/ScriptObjectManagerV8.h>
#include <Script/JS/V8/ScriptUtilsV8.h>
#include <Script/Templates/ScriptObjectTemplate.h>
#include <v8-external.h>
#include <v8-function.h>
#include <v8-object.h>

namespace Zeron::Script
{

	ObjectStorageV8::ObjectStorageV8(ContextV8* context, ObjectManagerV8* manager, const ObjectTemplate& objTemplate)
		: mContext(context)
		, mTypeInfo(objTemplate.mTypeInfo)
		, mNativeTemplate(objTemplate)
		, mConstructorContext{ manager, &mNativeTemplate }
	{
		v8::Isolate* isolate = context->GetIsolateV8();

		const v8::Local<v8::FunctionTemplate> constructor = mNativeTemplate.mConstructor.has_value() ?
			v8::FunctionTemplate::New(isolate, UtilsV8::CreateConstructorCallback(), v8::External::New(isolate, &mConstructorContext)) :
			v8::FunctionTemplate::New(isolate, UtilsV8::CreateConstructorCallback());
		constructor->InstanceTemplate()->SetInternalFieldCount(2);

		for (auto& [name, closure] : mNativeTemplate.mFunctions) {
			constructor->PrototypeTemplate()->Set(
				isolate, name.c_str(), v8::FunctionTemplate::New(isolate, UtilsV8::CreateFunctionCallback(), v8::External::New(isolate, &closure))
			);
		}

		for (auto& [name, prop] : mNativeTemplate.mProperties) {
			ZE_ASSERT(prop.mGetter, "Expected Script property getter to not be null");
			const bool isReadOnly = prop.mSetter == nullptr;
			constructor->InstanceTemplate()->SetAccessor(
				UtilsV8::FromString(isolate, name),
				UtilsV8::CreateGetterCallback(),
				isReadOnly ? nullptr : UtilsV8::CreateSetterCallback(),
				v8::External::New(context->GetIsolateV8(), &prop),
				v8::DEFAULT,
				isReadOnly ? static_cast<v8::PropertyAttribute>(v8::ReadOnly | v8::DontDelete) : v8::DontDelete
			);
		}

		mJSTemplate.Reset(context->GetIsolateV8(), constructor);
	}

	ObjectStorageV8::~ObjectStorageV8() {}

	ObjectInstance::Handle ObjectStorageV8::GetOrAddInstance(ObjectInstance::Handle handle, ObjectInstance::Owner owner = nullptr)
	{
		auto existing = mInstances.find(handle);
		if (existing != mInstances.end()) {
			return existing->first;
		}

		v8::Local<v8::Object> obj;
		// Create object without calling bound constructor
		if (mJSTemplate.Get(mContext->GetIsolateV8())->InstanceTemplate()->NewInstance(mContext->GetContextV8()).ToLocal(&obj)) {
			obj->SetAlignedPointerInInternalField(0, handle);
			obj->SetAlignedPointerInInternalField(1, this);

			v8::Global<v8::Object> persistentObj(mContext->GetIsolateV8(), obj);
			// This will be called when the object is garbage collected
			persistentObj.SetWeak(
				this,
				[](const v8::WeakCallbackInfo<ObjectStorageV8>& data) {
					ObjectInstance::Handle handle = data.GetInternalField(0);
					auto storage = static_cast<ObjectStorageV8*>(data.GetInternalField(1));
					if (storage->mDestructor) {
						storage->mDestructor(handle);
					}
					const Instance* instance = storage->GetInstance(handle);
					if (instance && instance->mOwner && instance->mOwner.use_count() <= 1) {
						storage->RemoveInstance(handle);
					}
				},
				v8::WeakCallbackType::kInternalFields
			);
			mInstances.emplace(handle, Instance{ std::move(persistentObj), std::move(owner) });
			return handle;
		}
		return nullptr;
	}
	const TypeInfo& ObjectStorageV8::GetTypeInfo() const
	{
		return mTypeInfo;
	}

	ObjectStorageV8::Instance* ObjectStorageV8::GetInstance(ObjectInstance::Handle handle)
	{
		auto obj = mInstances.find(handle);
		if (obj != mInstances.end()) {
			return &obj->second;
		}
		return nullptr;
	}

	v8::Local<v8::Object> ObjectStorageV8::GetInstanceV8(ObjectInstance::Handle handle)
	{
		auto obj = mInstances.find(handle);
		if (obj != mInstances.end()) {
			return obj->second.mInstance.Get(mContext->GetIsolateV8());
		}
		return {};
	}
	void ObjectStorageV8::RemoveInstance(ObjectInstance::Handle handle)
	{
		mInstances.erase(handle);
	}

}