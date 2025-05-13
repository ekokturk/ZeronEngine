// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

// Platform
#if ZE_PLATFORM_WIN32
#	define NOMINMAX
#	include <winsock2.h>
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
#include <unordered_set>
#include <variant>

// Zeron Core
#include <Core/Diag/Assert.h>
#include <Core/Diag/Logger.h>
#include <Core/Locator.h>
#include <Core/SystemHandle.h>
#include <Core/Types/Types.h>
#include <Core/Util.h>
#include <Math/Math.h>