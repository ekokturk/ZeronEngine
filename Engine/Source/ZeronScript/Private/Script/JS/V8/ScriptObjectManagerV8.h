// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

#include <Script/ScriptObjectManager.h>
#include <Script/Templates/ScriptObjectTemplate.h>
#include <v8-local-handle.h>

namespace v8
{
	class FunctionTemplate;
}

namespace Zeron::Script
{
	class ObjectStorageV8;
	class ContextV8;

	class ObjectManagerV8 : public ObjectManager {
	  public:
		ObjectManagerV8(ContextV8* ctx);
		~ObjectManagerV8() override;

		v8::Local<v8::FunctionTemplate> Register(const ObjectTemplate& objTemplate);

		ObjectStorageV8* GetStorageV8(const size_t& typeId);

	  private:
		ContextV8* mContext;
	};

}