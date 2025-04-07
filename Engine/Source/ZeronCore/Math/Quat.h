// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

namespace Zeron
{
	template <typename T>
	using TQuat = glm::tquat<T, glm::highp>;

	using Quat = TQuat<float>;
}