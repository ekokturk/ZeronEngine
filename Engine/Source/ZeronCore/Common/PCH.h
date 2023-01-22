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
#include <Common/Assert.h>
#include <Common/Logger.h>
#include <Common/SystemHandle.h>
#include <Common/TypeTraits.h>
#include <Common/Math/Math.h>
#include <Common/Types/Color.h>
#include <Common/Types/Enum.h>
#include <Common/Types/SparseSet.h>