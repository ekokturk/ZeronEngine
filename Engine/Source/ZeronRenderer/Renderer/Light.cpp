// Copyright (C) 2020, Eser Kokturk. All Rights Reserved.

#include "Light.h"

namespace Zeron
{
	Light::Light()
	{
		
	}

	Light::Light(Graphics& graphics)
	{
		// TODO: Create light model resources here
	}

	void Light::Draw(GraphicsContext& ctx, Camera& camera, const Mat4& worldMatrix)
	{
		// TODO: Draw Light Model here
	}

	void Light::SetPosition(const Vec3& pos)
	{
		mPosition = pos;
	}

	void Light::SetColor(const Vec3& color)
	{
		mColor = color;
	}

	void Light::SetStrength(float strength)
	{
		mStrength = strength;
	}

	void Light::SetAttenuation(const Vec3& attenuation)
	{
		mAttenuation = attenuation;
	}
}
