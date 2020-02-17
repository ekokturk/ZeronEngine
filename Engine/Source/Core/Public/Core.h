// Copyright (C) 2020, Eser Kokturk. All Rights Reserved.

#pragma once

#ifdef ZeronEngine_EXPORTS
#define ZRN_API  __declspec(dllexport)
#else
#define ZRN_API  __declspec(dllimport)
#endif