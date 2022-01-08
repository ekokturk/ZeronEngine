// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once
#include <glm/detail/type_mat3x3.hpp>

namespace Zeron
{
	template<typename T>
	using TMat3 = glm::tmat3x3<T, glm::highp>;

	using Mat3 = TMat3<float>;
	using Mat3i = TMat3<int>;
	using Mat3d = TMat3<double>;
}
