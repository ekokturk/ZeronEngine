// Copyright (C) Eser Kokturk. All Rights Reserved.

#include <ZeronTestFramework.h>

#include <Script/ScriptContext.h>
#include <Script/ScriptEngine.h>
#include <Script/ScriptRuntime.h>
#include <Script/ScriptRuntimeType.h>

using namespace ::Zeron;

namespace ZeronScriptTests::JavascriptTests
{
	TEST(NativeObject, BindingWithFunctions_CallFunctionsExistingInstance_ReturnedValue)
	{
		struct CustomObject {
			void FnVoid() { ++mFunctionWithReturnCalls; }

			std::string FnWithReturn()
			{
				++mFunctionCalls;
				return "customValue";
			}

			int mFunctionCalls = 0;
			int mFunctionWithReturnCalls = 0;
		};
		CustomObject* objectInstance = nullptr;
		Script::ObjectTemplate consoleObject = Script::ObjectTemplate::Create<CustomObject>();
		consoleObject.AddFunction("CallFnVoid", &CustomObject::FnVoid).AddFunction("CallFnWithReturn", &CustomObject::FnWithReturn);
		Script::GlobalTemplate globalTemplate;
		globalTemplate.AddObjectInstance<CustomObject>("customObject", [&objectInstance](Script::ObjectManager& manager) {
			auto instance = manager.CreateInstance<CustomObject>();
			auto storage = manager.GetStorage<CustomObject>();
			EXPECT_TRUE(storage);
			objectInstance = static_cast<CustomObject*>(instance.mHandle);
			return instance;
		});
		globalTemplate.AddClass("CustomObject", std::move(consoleObject));
		Script::ScriptEngine engine;
		auto runtime = engine.CreateRuntime(Script::RuntimeType::JavaScriptV8);
		auto context = runtime->CreateContext(std::move(globalTemplate));

		const Script::Value result = context->Run(R"(
			customObject.CallFnVoid()
			customObject.CallFnWithReturn()
		)");


		ASSERT_TRUE(objectInstance);
		EXPECT_EQ("customValue", result.Get<std::string>());
		EXPECT_EQ(1, objectInstance->mFunctionCalls);
		EXPECT_EQ(1, objectInstance->mFunctionWithReturnCalls);
	}

	TEST(NativeObject, BindingWithProp_TryModifiyingProp_PropChanged)
	{
		struct CustomObject {
			std::string mProp = "defaultProp";
		};
		CustomObject* objectInstance = nullptr;
		Script::ObjectTemplate consoleObject = Script::ObjectTemplate::Create<CustomObject>();
		consoleObject.AddProperty("prop", &CustomObject::mProp);
		Script::GlobalTemplate globalTemplate;
		globalTemplate.AddObjectInstance<CustomObject>("customObject", [&objectInstance](Script::ObjectManager& manager) {
			auto instance = manager.CreateInstance<CustomObject>();
			auto storage = manager.GetStorage<CustomObject>();
			EXPECT_TRUE(storage);
			objectInstance = static_cast<CustomObject*>(instance.mHandle);
			return instance;
		});
		globalTemplate.AddClass("CustomObject", std::move(consoleObject));
		Script::ScriptEngine engine;
		auto runtime = engine.CreateRuntime(Script::RuntimeType::JavaScriptV8);
		auto context = runtime->CreateContext(std::move(globalTemplate));

		const Script::Value result = context->Run(R"(
			customObject.prop = 'updatedProp'
			customObject.prop
		)");

		ASSERT_TRUE(objectInstance);
		EXPECT_EQ("updatedProp", result.Get<std::string>());
	}

	TEST(NativeObject, BindingWithReadOnlyProp_TryModifiyingProp_PropNotChanged)
	{
		struct CustomObject {
			std::string mProp = "defaultProp";
		};
		CustomObject* objectInstance = nullptr;
		Script::ObjectTemplate consoleObject = Script::ObjectTemplate::Create<CustomObject>();
		consoleObject.AddPropertyReadOnly("prop", &CustomObject::mProp);
		Script::GlobalTemplate globalTemplate;
		globalTemplate.AddObjectInstance<CustomObject>("customObject", [&objectInstance](Script::ObjectManager& manager) {
			auto instance = manager.CreateInstance<CustomObject>();
			auto storage = manager.GetStorage<CustomObject>();
			EXPECT_TRUE(storage);
			objectInstance = static_cast<CustomObject*>(instance.mHandle);
			return instance;
		});
		globalTemplate.AddClass("CustomObject", std::move(consoleObject));
		Script::ScriptEngine engine;
		auto runtime = engine.CreateRuntime(Script::RuntimeType::JavaScriptV8);
		auto context = runtime->CreateContext(std::move(globalTemplate));

		const Script::Value result = context->Run(R"(
			customObject.prop = 'updatedProp'
			customObject.prop
		)");

		ASSERT_TRUE(objectInstance);
		EXPECT_EQ("defaultProp", result.Get<std::string>());
	}


	TEST(NativeObject, BindingWithNoConstructor_ConstructObject_Error)
	{
		struct CustomObject {};
		Script::ObjectTemplate consoleObject = Script::ObjectTemplate::Create<CustomObject>();
		Script::GlobalTemplate globalTemplate;
		globalTemplate.AddClass("CustomObject", std::move(consoleObject));
		Script::ScriptEngine engine;
		auto runtime = engine.CreateRuntime(Script::RuntimeType::JavaScriptV8);
		auto context = runtime->CreateContext(std::move(globalTemplate));

		const Script::Value result = context->Run(R"(
			try {
				const obj = new CustomObject()
			}
			catch {
				'Error Thrown'			
			}
		)");

		EXPECT_EQ("Error Thrown", result.Get<std::string>());
	}

	TEST(NativeObject, BindingWithConstructor_ConstructObject_ObjectExists)
	{
		struct CustomObject {};
		Script::ObjectTemplate consoleObject = Script::ObjectTemplate::Create<CustomObject>().AddConstructor<CustomObject>();
		Script::GlobalTemplate globalTemplate;
		globalTemplate.AddClass("CustomObject", std::move(consoleObject));
		Script::ScriptEngine engine;
		auto runtime = engine.CreateRuntime(Script::RuntimeType::JavaScriptV8);
		auto context = runtime->CreateContext(std::move(globalTemplate));

		const Script::Value result = context->Run(R"(
			const obj = new CustomObject()
			typeof obj === 'object'
		)");

		EXPECT_EQ(true, result.Get<bool>());
	}
}