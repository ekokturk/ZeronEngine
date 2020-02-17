// Copyright (C) 2020, Eser Kokturk. All Rights Reserved.

#pragma once

class Application
{
private:

public:
    Application();
    virtual ~Application();
    
    virtual void Run();
    virtual void Destroy();
};

