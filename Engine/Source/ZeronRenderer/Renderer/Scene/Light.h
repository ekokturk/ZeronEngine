// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

namespace Zeron
{
	class Camera;
	class GraphicsContext;
	class Graphics;

	class Light {
	  public:
		Light();
		Light(Graphics& graphics);

		void Draw(GraphicsContext& ctx, Camera& camera, const Mat4& worldMatrix);

		// TODO: These should go into light actor instead
		void SetPosition(const Vec3& pos);
		const Vec3& GetPosition() const { return mPosition; }

		void SetColor(const Vec3& color);
		void SetStrength(float strength);
		void SetAttenuation(const Vec3& attenuation);

		const Vec3& GetColor() const { return mColor; }
		float GetStrength() const { return mStrength; }
		const Vec3& GetAttenuation() const { return mAttenuation; }

	  private:
		Vec3 mPosition;
		Vec3 mColor = Vec3::ONE;
		float mStrength = 1.f;
		Vec3 mAttenuation = Vec3{ 1.f, 0.1f, 0.1f };
	};
}