// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

#include <Script/ScriptObjectStorage.h>
#include <Script/Templates/ScriptObjectTemplate.h>
#include <v8-persistent-handle.h>

namespace v8
{
	class FunctionTemplate;
}

namespace Zeron::Script
{
	class ObjectManager;
	class ObjectManagerV8;
	class ContextV8;

	struct ObjectConstructorContextV8 {
		ObjectManager* mManager = nullptr;
		ObjectTemplate* mTemplate = nullptr;
	};

	class ObjectStorageV8 : public ObjectStorage {
	  public:
		struct Instance {
			v8::Global<v8::Object> mInstance;
			std::shared_ptr<void> mOwner;
		};

		ObjectStorageV8(ContextV8* context, ObjectManagerV8* manager, const ObjectTemplate& objTemplate);
		~ObjectStorageV8() override;

		ObjectInstance::Handle GetOrAddInstance(ObjectInstance::Handle handle, ObjectInstance::Owner owner) override;
		const TypeInfo& GetTypeInfo() const override;
		Instance* GetInstance(ObjectInstance::Handle handle);
		void RemoveInstance(ObjectInstance::Handle handle);

		v8::Local<v8::Object> GetInstanceV8(ObjectInstance::Handle handle);

		ContextV8* mContext;
		TypeInfo mTypeInfo;
		std::function<void(ObjectInstance::Handle)> mDestructor = nullptr;
		ObjectTemplate mNativeTemplate;
		v8::Global<v8::FunctionTemplate> mJSTemplate;
		std::unordered_map<ObjectInstance::Handle, Instance> mInstances;
		ObjectConstructorContextV8 mConstructorContext;
	};

}