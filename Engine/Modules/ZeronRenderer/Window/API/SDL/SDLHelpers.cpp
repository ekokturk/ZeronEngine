// Copyright (C) Eser Kokturk. All Rights Reserved.

#if ZE_WINDOW_SDL
#include <Window/API/SDL/SDLHelpers.h>

#if ZE_PLATFORM_LINUX
    #define SDL_VIDEO_DRIVER_X11
#endif
#include <SDL.h>
#include <SDL_syswm.h>

namespace Zeron {

    SystemHandle SDLHelpers::GetPlatformWindowHandle(SDL_Window* sdlWindow) {
        SystemHandle handle;
#if ZE_PLATFORM_WIN32
        SDL_SysWMinfo wmInfo;
        SDL_VERSION(&wmInfo.version);
        SDL_GetWindowWMInfo(sdlWindow, &wmInfo);
        handle.mWindow = wmInfo.info.win.window;
#elif ZE_PLATFORM_LINUX
        SDL_SysWMinfo wmInfo;
        SDL_VERSION(&wmInfo.version);
        SDL_GetWindowWMInfo(sdlWindow, &wmInfo);
        handle.mWindow = wmInfo.info.x11.window;		
        handle.mConnection = wmInfo.info.x11.display;		
#elif ZE_PLATFORM_ANDROID
        SDL_SysWMinfo wmInfo;
        SDL_VERSION(&wmInfo.version);
        SDL_GetWindowWMInfo(sdlWindow, &wmInfo);
        handle.mWindow = wmInfo.info.android.window;		
#endif
        return handle;
    }

}

#endif
