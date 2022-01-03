// Copyright (C) 2020, Eser Kokturk. All Rights Reserved.

#pragma once

namespace Zeron
{
	enum class ProjectionType {
		Perspective,
		Orthographic
	};

	
	class Camera {
	public:
		Camera();
		Camera(const Vec2& viewSize, float clipStart, float clipEnd);
		Camera(float fovDegrees, const Vec2& viewSize, float clipStart, float clipEnd);
		virtual ~Camera();

		void Move(const Vec3& posStep);
		void Rotate(const Vec3& rotStep);
		void LookAt(const Vec3& target);
		void SetPosition(const Vec3& pos);
		void SetRotation(const Vec3& rot);
		void SetFieldOfView(float fovDegrees);
		void SetViewSize(const Vec2& size);
		void SetClipPositions(float clipStart, float clipEnd);
		void SetProjectionType(ProjectionType type);

		[[nodiscard]] const Vec3& GetPosition() const;
		[[nodiscard]] const Vec3& GetRotation() const;
		[[nodiscard]] const Vec3& GetForwardDir() const;
		[[nodiscard]] const Vec3& GetRightDir() const;
		[[nodiscard]] const Vec3& GetUpDir() const;
		[[nodiscard]] float GetFieldOfView() const;
		[[nodiscard]] const Vec2& GetViewSize() const;
		[[nodiscard]] const Vec2& GetClipPositions() const;
		[[nodiscard]] const Mat4& GetViewMatrix() const;
		[[nodiscard]] const Mat4& GetProjectionMatrix() const;
		[[nodiscard]] ProjectionType GetProjectionType() const;
		

	private:
		void UpdateProjection_();
		void UpdateView_();

		Vec3 mPos;
		Vec3 mRot;
		float mFOV;
		Vec2 mViewSize;
		Vec2 mClip;
		Mat4 mProjectionMatrix;
		Mat4 mViewMatrix;
		Vec3 mForwardDir;
		Vec3 mRightDir;
		Vec3 mUpDir;
		ProjectionType mProjectionType;
	};
}
