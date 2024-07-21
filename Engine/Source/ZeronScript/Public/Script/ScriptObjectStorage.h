// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

#include <Script/ScriptObject.h>
#include <Script/TypeInfo.h>

namespace Zeron::Script
{
	class ObjectStorage {
	  public:
		virtual ~ObjectStorage() = default;
		virtual ObjectInstance::Handle GetOrAddInstance(ObjectInstance::Handle handle, ObjectInstance::Owner owner) = 0;
		virtual const TypeInfo& GetTypeInfo() const = 0;
	};
}