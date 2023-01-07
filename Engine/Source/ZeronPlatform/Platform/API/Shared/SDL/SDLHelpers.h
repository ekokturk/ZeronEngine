// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once
#include <Input/KeyCode.h>
#include <Input/MouseCode.h>

#if ZE_WINDOW_SDL

struct SDL_Window;

namespace Zeron {
    class SDLHelpers {
    public:
        static SystemHandle GetPlatformWindowHandle(SDL_Window* sdlWindow);
        static KeyCode GetKeyCode(int32_t code);
        static MouseCode GetMouseCode(int32_t code);
    };
}

#endif