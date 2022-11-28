// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

#if ZE_WINDOW_SDL

struct SDL_Window;

namespace Zeron {
    class SDLHelpers {
    public:
        static SystemHandle GetPlatformWindowHandle(SDL_Window* sdlWindow);
    };
}

#endif