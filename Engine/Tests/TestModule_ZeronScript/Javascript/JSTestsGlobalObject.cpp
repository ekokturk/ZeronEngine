// Copyright (C) Eser Kokturk. All Rights Reserved.

#include <ZeronTestFramework.h>

#include <Script/ScriptContext.h>
#include <Script/ScriptEngine.h>
#include <Script/ScriptRuntime.h>
#include <Script/ScriptRuntimeType.h>

using namespace ::Zeron;

namespace TestModule_ZeronScript::JSTests
{
	int mFunctionCallCount = 0;
	int mFunctionWithReturnCallCount = 0;
	int mFunctionWithReturnAndArgsCallCount = 0;

	void FnVoid()
	{
		++mFunctionCallCount;
	}

	std::string FnWithReturn()
	{
		++mFunctionWithReturnCallCount;
		return "returnValue";
	}

	std::string FnWithReturnAndArgs(const std::string& value)
	{
		++mFunctionWithReturnAndArgsCallCount;
		return value;
	}

	TEST(GlobalObject, BindingWithFunctions_CallFunctions_FunctionsCalled)
	{
		Script::ScriptEngine engine;
		Script::GlobalTemplate globalTemplate;
		globalTemplate.AddFunction("CallFnVoid", &FnVoid).AddFunction("CallFnWithReturn", &FnWithReturn);
		auto runtime = engine.CreateRuntime(Script::RuntimeType::JavaScriptV8);
		auto context = runtime->CreateContext(std::move(globalTemplate));

		const Script::Value result = context->Run(R"(
			CallFnVoid()
			CallFnWithReturn()
		)");


		EXPECT_EQ("returnValue", result.Get<std::string>());
		EXPECT_EQ(1, mFunctionCallCount);
		EXPECT_EQ(1, mFunctionWithReturnCallCount);
		mFunctionCallCount = 0;
		mFunctionWithReturnCallCount = 0;
	}

	TEST(GlobalObject, BindingWithArgFunction_CallWithArgs_FunctionCalled)
	{
		Script::ScriptEngine engine;
		Script::GlobalTemplate globalTemplate;
		globalTemplate.AddFunction("FnWithReturnAndArgs", &FnWithReturnAndArgs);
		auto runtime = engine.CreateRuntime(Script::RuntimeType::JavaScriptV8);
		auto context = runtime->CreateContext(std::move(globalTemplate));

		const Script::Value result = context->Run(R"(
			FnWithReturnAndArgs('customReturn')
		)");


		EXPECT_EQ("customReturn", result.Get<std::string>());
		EXPECT_EQ(1, mFunctionWithReturnAndArgsCallCount);
		mFunctionWithReturnAndArgsCallCount = 0;
	}

}