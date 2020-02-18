// Copyright (C) 2020, Eser Kokturk. All Rights Reserved.
#pragma once

#include "EntryPoint.h"
#include <iostream>


class Sandbox : public ZeronEngine::Application
{
public:
	Sandbox(){}
	~Sandbox() {}

	void Run() { std::cout << "Test"; };
};

CREATE_ZERON_APP(Sandbox) 