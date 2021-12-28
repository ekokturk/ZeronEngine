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
		[[nodiscard]] static TMat4<T> Transpose(const TMat4<T>& m) {
			return glm::transpose(m);
		}
		
		template<typename T>
		[[nodiscard]] static TMat3<T> Transpose(const TMat3<T>& m) {
			return glm::transpose(m);
		}
		
		template<typename T>
		[[nodiscard]] static TMat4<T> Translate(const TMat4<T>& m, const TVec3<T>& v) {
			return glm::translate(m, glm::vec3(v.X,v.Y, v.Z));
		}

		template<typename T>
		[[nodiscard]] static TMat4<T> Rotate(const TMat4<T>& m, T angle, const TVec3<T>& axis){
			return glm::rotate(m, angle, glm::vec3(axis.X, axis.Y, axis.Z));
		}

		template<typename T>
		[[nodiscard]] static TMat4<T> Scale(const TMat4<T>& m, const TVec3<T>& axis) {
			return glm::scale(m, glm::vec3(axis.X, axis.Y, axis.Z));
		}

	};

}