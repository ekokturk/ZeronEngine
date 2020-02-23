// Copyright (C) 2020, Eser Kokturk. All Rights Reserved.
#pragma once

#include <iostream>
#include "Application.h"


class Sandbox : public ZeronEngine::Application
{
public:
	void Run() { ZERON_LOG("Sandbox Running!") };
};

