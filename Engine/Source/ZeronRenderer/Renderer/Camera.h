// Copyright (C) 2020, Eser Kokturk. All Rights Reserved.

#pragma once

namespace Zeron
{
	class Camera {
	public:
		Camera();
		Camera(float fovDegrees, float aspectRatio, float clipStart, float clipEnd);
		virtual ~Camera();

		void Move(const Vec3& posStep);
		void Rotate(const Vec3& rotStep);
		void LookAt(const Vec3& target);
		void SetPosition(const Vec3& pos);
		void SetRotation(const Vec3& rot);
		void SetFieldOfView(float fovDegrees);
		void SetAspectRatio(float aspectRatio);
		void SetClipPositions(float clipStart, float clipEnd);

		[[nodiscard]] const Vec3& GetPosition() const;
		[[nodiscard]] const Vec3& GetRotation() const;
		[[nodiscard]] const Vec3& GetForwardDir() const;
		[[nodiscard]] const Vec3& GetRightDir() const;
		[[nodiscard]] const Vec3& GetUpDir() const;
		[[nodiscard]] float GetFieldOfView() const;
		[[nodiscard]] float GetAspectRatio() const;
		[[nodiscard]] const Vec2& GetClipPositions() const;
		[[nodiscard]] const Mat4& GetViewMatrix() const;
		[[nodiscard]] const Mat4& GetProjectionMatrix() const;
		

	private:
		void UpdateProjection_();
		void UpdateView_();

		Vec3 mPos;
		Vec3 mRot;
		float mFOV;
		float mAspectRatio;
		Vec2 mClip;
		Mat4 mProjectionMatrix;
		Mat4 mViewMatrix;
		Vec3 mForwardDir;
		Vec3 mRightDir;
		Vec3 mUpDir;
		
	};
}
