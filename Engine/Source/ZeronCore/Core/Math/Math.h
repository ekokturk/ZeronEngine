// Copyright (C) 2020, Eser Kokturk. All Rights Reserved.

#pragma once

#include "Core/Math/Vec2.h"
#include "Core/Math/Vec3.h"
#include "Core/Math/Vec4.h"
#include "Core/Math/Mat3.h"
#include "Core/Math/Mat4.h"

#include "glm/glm.hpp"
#include "glm/gtx/transform.hpp"

namespace Zeron
{
	struct Math {

		// ----------------- GLM Wrappers -----------------
		template<typename T>
		[[nodiscard]] static T ToRadians(T degrees) {
			return glm::radians(degrees);
		}

		template<typename T>
		[[nodiscard]] static T ToDegrees(T radians) {
			return glm::degrees(radians);
		}
		
		template<typename T>
		[[nodiscard]] static TMat4<T> Transpose(const TMat4<T>& m) {
			return glm::transpose(m);
		}
		
		template<typename T>
		[[nodiscard]] static TMat3<T> Transpose(const TMat3<T>& m) {
			return glm::transpose(m);
		}
		
		template<typename T>
		[[nodiscard]] static TMat4<T> Translate(const TMat4<T>& m, const TVec3<T>& v) {
			return glm::translate(m, glm::tvec3<T>(v.X,v.Y, v.Z));
		}

		template<typename T>
		[[nodiscard]] static TMat4<T> Rotate(const TMat4<T>& m, T angle, const TVec3<T>& axis){
			return glm::rotate(m, angle, glm::tvec3<T>(axis.X, axis.Y, axis.Z));
		}

		template<typename T>
		[[nodiscard]] static TMat4<T> Scale(const TMat4<T>& m, const TVec3<T>& axis) {
			return glm::scale(m, glm::tvec3<T>(axis.X, axis.Y, axis.Z));
		}

		template<typename T>
		[[nodiscard]] static TMat4<T> LookAt(const TVec3<T>& eye, const TVec3<T>& center, const TVec3<T>& up) {
			return glm::lookAtLH(glm::tvec3<T>{ eye.X, eye.Y, eye.Z },
				glm::tvec3<T>{ center.X, center.Y, center.Z },
				glm::tvec3<T>{up.X, up.Y, up.Z});
		}

		template<typename T>
		[[nodiscard]] static TMat4<T> Perspective(T fov, T aspect, T nearZ, T farZ) {
			return glm::perspectiveLH(fov, aspect, nearZ, farZ);
		}
		
		template<typename T>
		[[nodiscard]] static TMat4<T> PerspectiveFOV(T fov, T width, T height, T nearZ, T farZ) {
			return glm::perspectiveFovLH(fov, width, height, nearZ, farZ);
		}
		
	};

}