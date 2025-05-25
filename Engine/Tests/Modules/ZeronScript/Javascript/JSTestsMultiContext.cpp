// Copyright (C) Eser Kokturk. All Rights Reserved.

#include <ZeronTestFramework.h>

#include <Script/ScriptContext.h>
#include <Script/ScriptEngine.h>
#include <Script/ScriptRuntime.h>
#include <Script/ScriptRuntimeType.h>

using namespace ::Zeron;

namespace ZeronScriptTests::JavascriptTests
{
	TEST(MultiContext, CreateTwoContexts_RunScriptForFirst_ReturnedValue)
	{
		Script::ScriptEngine engine;
		auto runtime = engine.CreateRuntime(Script::RuntimeType::JavaScriptV8);
		auto context1 = runtime->CreateContext();
		EXPECT_TRUE(runtime->IsInScope(context1));
		auto context2 = runtime->CreateContext();
		EXPECT_TRUE(runtime->IsInScope(context2));
		EXPECT_FALSE(runtime->IsInScope(context1));

		runtime->EnterScope(context1);
		EXPECT_TRUE(runtime->IsInScope(context1));
		EXPECT_FALSE(runtime->IsInScope(context2));
		const Script::Value result = context1->Run(R"(
			true
		)");

		EXPECT_EQ(true, result.Get<bool>());
	}

}