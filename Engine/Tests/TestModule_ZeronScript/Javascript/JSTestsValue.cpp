// Copyright (C) Eser Kokturk. All Rights Reserved.

#include <ZeronTestFramework.h>

#include <Script/ScriptContext.h>
#include <Script/ScriptEngine.h>
#include <Script/ScriptRuntime.h>
#include <Script/ScriptRuntimeType.h>

using namespace ::Zeron;

namespace TestModule_ZeronScript::JSTests
{
	TEST(Value, Null_RunWithUndefinedResult_CorrectValue)
	{
		Script::ScriptEngine engine;
		auto runtime = engine.CreateRuntime(Script::RuntimeType::JavaScriptV8);
		auto context = runtime->CreateContext();

		const Script::Value result = context->Run(R"(
			undefined
		)");

		EXPECT_TRUE(result.IsType<void>());
	}

	TEST(Value, Null_RunWithNullResult_CorrectValue)
	{
		Script::ScriptEngine engine;
		auto runtime = engine.CreateRuntime(Script::RuntimeType::JavaScriptV8);
		auto context = runtime->CreateContext();

		const Script::Value result = context->Run(R"(
			null
		)");

		EXPECT_TRUE(result.IsType<void>());
	}

	TEST(Value, String_RunWithStringResult_CorrectValue)
	{
		Script::ScriptEngine engine;
		auto runtime = engine.CreateRuntime(Script::RuntimeType::JavaScriptV8);
		auto context = runtime->CreateContext();

		const Script::Value result = context->Run(R"(
			'test' + 'String'
		)");

		EXPECT_EQ("testString", result.Get<std::string>());
	}

	TEST(Value, Bool_RunWithTrueBooleanResult_CorrectValue)
	{
		Script::ScriptEngine engine;
		auto runtime = engine.CreateRuntime(Script::RuntimeType::JavaScriptV8);
		auto context = runtime->CreateContext();

		const Script::Value result = context->Run(R"(
			true
		)");

		EXPECT_EQ(true, result.Get<bool>());
	}

	TEST(Value, Bool_RunWithFalseBooleanResult_CorrectValue)
	{
		Script::ScriptEngine engine;
		auto runtime = engine.CreateRuntime(Script::RuntimeType::JavaScriptV8);
		auto context = runtime->CreateContext();

		const Script::Value result = context->Run(R"(
			false
		)");

		EXPECT_EQ(false, result.Get<bool>());
	}
}