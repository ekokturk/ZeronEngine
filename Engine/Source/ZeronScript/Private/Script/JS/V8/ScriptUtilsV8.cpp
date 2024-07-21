// Copyright (C) Eser Kokturk. All Rights Reserved.

#include <Script/JS/V8/ScriptUtilsV8.h>

#include <Script/JS/V8/ScriptObjectManagerV8.h>
#include <Script/JS/V8/ScriptObjectStorageV8.h>
#include <Script/ScriptObject.h>
#include <v8-exception.h>
#include <v8-external.h>
#include <v8-function.h>
#include <v8-object.h>
#include <v8-platform.h>

namespace Zeron::Script
{
	v8::Local<v8::String> UtilsV8::FromString(v8::Isolate* isolate, std::string_view str)
	{
		return v8::String::NewFromUtf8(isolate, str.data(), v8::NewStringType::kNormal, static_cast<int>(str.size())).ToLocalChecked();
	}

	Value UtilsV8::ToValue(v8::Isolate* isolate, v8::Local<v8::Value> val)
	{
		if (val->IsNullOrUndefined()) {
			return {};
		}
		if (val->IsBoolean()) {
			return val->BooleanValue(isolate);
		}
		if (val->IsNumber()) {
			// TODO: Different number formats
		}
		if (val->IsString()) {
			v8::String::Utf8Value message(isolate, val);
			return std::string(*message);
		}
		if (val->IsObject()) {
			v8::Local<v8::Object> obj = val.As<v8::Object>();
			// Native bound object
			if (obj->InternalFieldCount() > 0) {
				ObjectInstance::Handle handle = obj->GetAlignedPointerFromInternalField(0);
				auto storage = static_cast<ObjectStorageV8*>(obj->GetAlignedPointerFromInternalField(1));
				const ObjectStorageV8::Instance* instance = storage->GetInstance(handle);
				if (instance) {
					return handle;
				}
			}
			else {
				// TODO: Custom object
			}
		}

		return {};
	}

	v8::Local<v8::Value> UtilsV8::FromValue(v8::Isolate* isolate, Value& val)
	{
		const TypeInfo& typeInfo = val.GetTypeInfo();

		if (!val.HasValue()) {
			return v8::Undefined(isolate);
		}
		else if (typeInfo.IsIntegral()) {
		}
		else if (typeInfo.IsEqualRaw<std::string>()) {
			const auto& str = val.Get<const std::string&>();
			return v8::String::NewFromUtf8(isolate, str.data(), v8::NewStringType::kNormal, static_cast<int>(str.size())).ToLocalChecked();
		}
		return {};
	}

	v8::AccessorGetterCallback UtilsV8::CreateGetterCallback()
	{
		return [](v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info) {
			v8::Isolate* isolate = info.GetIsolate();
			const v8::Local<v8::External> external = v8::Local<v8::External>::Cast(info.Data());
			const PropertyTemplate* prop = static_cast<PropertyTemplate*>(external->Value());
			ZE_ASSERT(prop, "Expected property template to exist");

			const v8::Local<v8::Object> jsObject = info.This();
			void* caller = jsObject->GetAlignedPointerFromInternalField(0);
			ZE_ASSERT(caller, "Expected Script Getter caller to exist");

			auto val = prop->mGetter(caller);
			info.GetReturnValue().Set(UtilsV8::FromValue(isolate, val));
		};
	}

	v8::AccessorSetterCallback UtilsV8::CreateSetterCallback()
	{
		return [](v8::Local<v8::String>, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info) {
			v8::Isolate* isolate = info.GetIsolate();
			v8::HandleScope handleScope(isolate);

			const v8::Local<v8::External> external = v8::Local<v8::External>::Cast(info.Data());
			const PropertyTemplate* prop = static_cast<PropertyTemplate*>(external->Value());


			const v8::Local<v8::Object> jsObject = info.This();
			void* caller = jsObject->GetAlignedPointerFromInternalField(0);
			ZE_ASSERT(caller, "Expected Script Setter caller to exist");

			prop->mSetter(UtilsV8::ToValue(isolate, value), caller);
		};
	}

	v8::FunctionCallback UtilsV8::CreateConstructorCallback()
	{
		return [](const v8::FunctionCallbackInfo<v8::Value>& args) {
			v8::Isolate* isolate = args.GetIsolate();
			v8::HandleScope handleScope(isolate);

			// Validate function
			const v8::Local<v8::External> external = v8::Local<v8::External>::Cast(args.Data());
			if (external->IsUndefined()) {
				UtilsV8::ThrowException(isolate, "Constructor is not implemented");
				return;
			}

			auto ctx = static_cast<ObjectConstructorContextV8*>(external->Value());
			auto manager = static_cast<ObjectManagerV8*>(ctx->mManager);
			const ObjectConstructorTemplate* constructor = &(*ctx->mTemplate->mConstructor);

			// Create argument list
			std::vector<Value> values;
			values.reserve(args.Length());
			for (int i = 0; i < args.Length(); ++i) {
				Value val = UtilsV8::ToValue(isolate, args[i]);
				// TODO: Check if we match arguments of the function
				values.emplace_back(std::move(val));
			}

			// Call Function
			const ObjectInstance instance = constructor->mCallback(*manager, std::move(values));
			if (instance.mHandle) {
				args.GetReturnValue().Set(manager->GetStorageV8(ctx->mTemplate->mTypeInfo.GetId())->GetInstanceV8(instance.mHandle));
			}
		};
	}

	v8::FunctionCallback UtilsV8::CreateFunctionCallback()
	{
		return [](const v8::FunctionCallbackInfo<v8::Value>& args) {
			v8::Isolate* isolate = args.GetIsolate();
			v8::HandleScope handleScope(isolate);

			// Validate function
			const v8::Local<v8::External> external = v8::Local<v8::External>::Cast(args.Data());
			const FunctionTemplate* closure = static_cast<FunctionTemplate*>(external->Value());
			if (!closure) {
				UtilsV8::ThrowException(isolate, "Expected function to be implemented!");
				return;
			}

			// Create argument list
			std::vector<Value> values;
			values.reserve(args.Length());
			for (int i = 0; i < args.Length(); ++i) {
				Value val = UtilsV8::ToValue(isolate, args[i]);
				// TODO: Check if we match arguments of the function
				values.emplace_back(std::move(val));
			}

			// Try getting caller object if one exists
			const v8::Local<v8::Object> jsObject = args.This();
			const bool hasCaller = jsObject->InternalFieldCount() > 0;
			void* caller = hasCaller ? jsObject->GetAlignedPointerFromInternalField(0) : nullptr;

			// Call Function
			Value val = closure->mCallback(values, caller);

			if (val.HasValue()) {
				args.GetReturnValue().Set(UtilsV8::FromValue(isolate, val));
			}
		};
	}

	void UtilsV8::ThrowException(v8::Isolate* isolate, std::string_view message)
	{
		const v8::Local<v8::Value> err = v8::Exception::Error(UtilsV8::FromString(isolate, message));
		isolate->ThrowException(err);

		// TODO: Return error type
	}


}