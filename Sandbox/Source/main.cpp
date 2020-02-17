// Copyright (C) 2020, Eser Kokturk. All Rights Reserved.


#include "Application.h"

int main(int argc, char ** args)
{
	ZeronEngine::Application * app = new ZeronEngine::Application();
	app->Run();
	
	delete app;
	
    return 0;
}
