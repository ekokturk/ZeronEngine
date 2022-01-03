// Copyright (C) 2020, Eser Kokturk. All Rights Reserved.

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
		[[nodiscard]] void SetPosition(const Vec3& pos);
		[[nodiscard]] const Vec3& GetPosition() const { return mPosition; }
		
		[[nodiscard]] void SetColor(const Vec3& color);
		[[nodiscard]] void SetStrength(float strength);
		[[nodiscard]] void SetAttenuation(const Vec3& attenuation);
		
		[[nodiscard]] const Vec3& GetColor() const { return mColor; }
		[[nodiscard]] float GetStrength() const { return mStrength; }
		[[nodiscard]] const Vec3& GetAttenuation() const { return mAttenuation; }

	private:
		Vec3 mPosition;
		Vec3 mColor = Vec3::ONE;
		float mStrength = 1.f;
		Vec3 mAttenuation = Vec3{ 1.f, 0.1f, 0.1f };
	};
}


