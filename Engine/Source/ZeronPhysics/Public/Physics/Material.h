// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

namespace Zeron::Physics
{
	class Material {
	  public:
		virtual ~Material();

		virtual void SetStaticFriction(float friction) = 0;
		virtual void SetDynamicFriction(float friction) = 0;
		virtual void SetBounciness(float bounciness) = 0;

		virtual float GetStaticFriction() const = 0;
		virtual float GetDynamicFriction() const = 0;
		virtual float GetBounciness() const = 0;
	};
}