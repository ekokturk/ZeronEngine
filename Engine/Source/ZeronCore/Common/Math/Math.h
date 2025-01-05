// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

#include <Common/Math/Mat3.h>
#include <Common/Math/Mat4.h>
#include <Common/Math/Vec2.h>
#include <Common/Math/Vec3.h>
#include <Common/Math/Vec4.h>
#include <glm/glm.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/transform.hpp>

namespace Zeron
{
	struct Math {

		// Global directions
		template <typename T>
		static const TVec3<T> FORWARD_DIR;

		template <typename T>
		static const TVec3<T> RIGHT_DIR;

		template <typename T>
		static const TVec3<T> UP_DIR;

		template <typename T>
		static bool EqualsNear(T value, T other, float precision = static_cast<T>(0.00001))
		{
			return std::abs(value - other) < precision;
		}

		// ----------------- GLM Wrappers -----------------
		template <typename T>
		static T PI()
		{
			return glm::pi<T>();
		}

		template <typename T>
		static T ToRadians(T degrees)
		{
			return glm::radians(degrees);
		}

		template <typename T>
		static T ToDegrees(T radians)
		{
			return glm::degrees(radians);
		}

		template <typename T>
		static TMat4<T> Transpose(const TMat4<T>& m)
		{
			return glm::transpose(m);
		}

		template <typename T>
		static TMat3<T> Transpose(const TMat3<T>& m)
		{
			return glm::transpose(m);
		}

		template <typename T>
		static TMat4<T> Translate(const TMat4<T>& m, const TVec3<T>& v)
		{
			return glm::translate(m, reinterpret_cast<const glm::tvec3<T>&>(v));
		}

		template <typename T>
		static TMat4<T> Rotate(const TMat4<T>& m, T angle, const TVec3<T>& axis)
		{
			return glm::rotate(m, angle, reinterpret_cast<const glm::tvec3<T>&>(axis));
		}

		template <typename T>
		static TMat4<T> Scale(const TMat4<T>& m, const TVec3<T>& size)
		{
			return glm::scale(m, reinterpret_cast<const glm::tvec3<T>&>(size));
		}

		template <typename T>
		static TMat4<T> LookAt(const TVec3<T>& eye, const TVec3<T>& center, const TVec3<T>& up)
		{
			return glm::lookAtLH(reinterpret_cast<const glm::tvec3<T>&>(eye), reinterpret_cast<const glm::tvec3<T>&>(center), reinterpret_cast<const glm::tvec3<T>&>(up));
		}

		template <typename T>
		static TMat4<T> Perspective(T fov, T aspect, T nearZ, T farZ)
		{
			return glm::perspectiveLH(fov, aspect, nearZ, farZ);
		}

		template <typename T>
		static TMat4<T> Orthographic(T left, T right, T bottom, T top, T nearZ, T farZ)
		{
			return glm::orthoLH(left, right, bottom, top, nearZ, farZ);
		}

		template <typename T>
		static TMat4<T> PerspectiveFOV(T fov, T width, T height, T nearZ, T farZ)
		{
			return glm::perspectiveFovLH(fov, width, height, nearZ, farZ);
		}

		template <typename T>
		static TMat4<T> RotationMatrixFromEuler(T pitch, T yaw, T roll)
		{
			return glm::eulerAngleYXZ(yaw, pitch, roll);
		}

		template <typename T>
		static TVec3<T> Transform(const TVec3<T>& v, const TMat4<T>& m)
		{
			const glm::tvec4<T> temp = m * glm::tvec4<T>(v.X, v.Y, v.Z, 1);
			return { temp.x, temp.y, temp.z };
		}

		template <typename T>
		static TVec4<T> Transform(const TVec4<T>& v, const TMat4<T>& m)
		{
			const glm::tvec4<T> temp = m * glm::tvec4<T>(v.X, v.Y, v.Z, v.W);
			return { temp.x, temp.y, temp.z, temp.w };
		}
	};

	template <class T>
	const TVec3<T> Math::RIGHT_DIR = TVec3<T>(1, 0, 0);

	template <class T>
	const TVec3<T> Math::UP_DIR = TVec3<T>(0, 1, 0);

	template <class T>
	const TVec3<T> Math::FORWARD_DIR = TVec3<T>(0, 0, 1);
}