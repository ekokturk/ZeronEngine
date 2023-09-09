// Copyright (C) Eser Kokturk. All Rights Reserved.

#include <Render/Light/Light.h>

namespace Zeron::Render
{
	Light::Light() {}

	Light::Light(Gfx::Graphics& graphics)
	{
		// TODO: Create light model resources here
	}

	void Light::Draw(Gfx::GraphicsContext& ctx, Camera& camera, const Mat4& worldMatrix)
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