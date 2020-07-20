// Copyright (C) 2020, Eser Kokturk. All Rights Reserved.

#pragma once

// Cmake will handle this
#ifdef ZeronEngine_EXPORTS
#define ZERON_API  __declspec(dllexport)
#else
#define ZERON_API  __declspec(dllimport)
#endif


