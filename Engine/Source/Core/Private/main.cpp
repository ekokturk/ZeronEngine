// Copyright (C) 2020, Eser Kokturk. All Rights Reserved.

#include "../Public/Application.h"
#include <memory>


int main(int argc, char* argv[])
{
    std::unique_ptr<Application> app = std::make_unique<Application>();

    app->Run();
}