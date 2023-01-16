// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

// Platform
#if ZE_PLATFORM_WIN32
#define NOMINMAX
#include <Windows.h>
#endif

// STL
#include <algorithm>
#include <any>
#include <array>
#include <bitset>
#include <cmath>
#include <cstdint>
#include <exception>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iostream>
#include <limits>
#include <map>
#include <memory>
#include <queue>
#include <set>
#include <sstream>
#include <unordered_map>
#include <variant>

// Zeron Core
#include <Core/Assert.h>
#include <Core/Logger.h>
#include <Core/SystemHandle.h>
#include <Core/TypeTraits.h>
#include <Core/Math/Math.h>
#include <Core/Types/Color.h>
#include <Core/Types/Enum.h>
#include <Core/Types/SparseSet.h>
