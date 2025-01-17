// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

// Platform
#if ZE_PLATFORM_WIN32
#	define NOMINMAX
#	include <Windows.h>
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
#include <optional>
#include <queue>
#include <random>
#include <regex>
#include <set>
#include <span>
#include <sstream>
#include <thread>
#include <time.h>
#include <typeindex>
#include <typeinfo>
#include <unordered_map>
#include <variant>

// Zeron Core
#include <Common/Assert.h>
#include <Common/Logger.h>
#include <Common/Math/Math.h>
#include <Common/NotNull.h>
#include <Common/Result.h>
#include <Common/SystemHandle.h>
#include <Common/Time.h>
#include <Common/Types/ByteBuffer.h>
#include <Common/Types/Color.h>
#include <Common/Types/Enum.h>
#include <Common/Types/Path.h>
#include <Common/Types/SparseSet.h>
#include <Common/TypeTraits.h>
#include <Common/Util.h>