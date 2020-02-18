// Copyright (C) 2020, Eser Kokturk. All Rights Reserved.
#pragma once

#include "../Public/Application.h"

extern ZeronEngine::Application* ZeronEngine::CreateApplication();

int main(int argc, char** argv)
{
	auto app = ZeronEngine::CreateApplication();
	app->Run();
	delete app;
}