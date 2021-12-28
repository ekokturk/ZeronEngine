// Copyright (C) 2020, Eser Kokturk. All Rights Reserved.

#pragma once
#include <glm/detail/type_mat.hpp>

namespace Zeron
{
	template<typename T>
	using TMat4 = glm::tmat4x4<T, glm::highp>;

	using Mat4 = TMat4<float>;
	using Mat4i = TMat4<int>;
	using Mat4d = TMat4<double>;
}
