// Copyright (C) 2020, Eser Kokturk. All Rights Reserved.

#include "Camera.h"

namespace Zeron
{
	Camera::Camera()
		: mFOV(90.f)
		, mAspectRatio(4.f/3)
		, mClip(0.01f, 10000.f)
	{
		UpdateProjection_();
		UpdateView_();
	}

	Camera::Camera(float fovDegrees, float aspectRatio, float clipStart, float clipEnd)
		: mFOV(fovDegrees)
		, mAspectRatio(aspectRatio)
	{
		SetClipPositions(clipStart, clipEnd); // Updates projection
		UpdateView_();
	}

	Camera::~Camera()
	{
	}

	void Camera::Move(const Vec3& posStep)
	{
		mPos += posStep;
		UpdateView_();
	}

	void Camera::Rotate(const Vec3& rotStep)
	{
		mRot += rotStep;
		UpdateView_();
	}

	void Camera::LookAt(const Vec3& target)
	{
		if(mPos == target) {
			return;
		}
		float pitch = 0.f;
		float yaw = 0.f;
		const Vec3 direction = mPos - target;
		if (!Math::EqualsNear(direction.Y, 0.f)) {
			const float distance = sqrt(direction.X * direction.X + direction.Z * direction.Z);
			pitch = atan(direction.Y / distance);
		}
		if (!Math::EqualsNear(direction.X, 0.f)) {
			yaw = atan(direction.X / direction.Z);
		}
		if(direction.Z > 0.f) {
			yaw += Math::PI<float>();
		}
		SetRotation({ pitch, yaw, 0.f });
	}

	void Camera::SetPosition(const Vec3& pos)
	{
		mPos = pos;
		UpdateView_();
	}

	void Camera::SetRotation(const Vec3& rot)
	{
		mRot = rot;
		UpdateView_();
	}

	void Camera::SetFieldOfView(float fovDegrees)
	{
		mFOV = fovDegrees;
		UpdateProjection_();
	}

	void Camera::SetAspectRatio(float aspectRatio)
	{
		mAspectRatio = aspectRatio;
		UpdateProjection_();
	}

	void Camera::SetClipPositions(float clipStart, float clipEnd)
	{
		mClip = { std::min(clipStart, clipEnd),
			std::max(clipStart, clipEnd) };
		UpdateProjection_();
	}

	const Vec3& Camera::GetPosition() const
	{
		return mPos;
	}

	const Vec3& Camera::GetRotation() const
	{
		return mRot;
	}

	const Vec3& Camera::GetForwardDir() const
	{
		return mForwardDir;
	}

	const Vec3& Camera::GetRightDir() const
	{
		return mRightDir;
	}

	const Vec3& Camera::GetUpDir() const
	{
		return mUpDir;
	}

	float Camera::GetFieldOfView() const
	{
		return mFOV;
	}

	float Camera::GetAspectRatio() const
	{
		return mAspectRatio;
	}

	const Vec2& Camera::GetClipPositions() const
	{
		return mClip;
	}

	const Mat4& Camera::GetViewMatrix() const
	{
		return mViewMatrix;
	}

	const Mat4& Camera::GetProjectionMatrix() const
	{
		return mProjectionMatrix;
	}

	void Camera::UpdateProjection_()
	{
		mProjectionMatrix = Math::Perspective(Math::ToRadians(mFOV), mAspectRatio, mClip.X, mClip.Y);
	}

	void Camera::UpdateView_()
	{
		const Mat4 rotMatrix = Math::RotationMatrixFromEuler(mRot.X, mRot.Y, mRot.Z);
		mUpDir = Math::Transform(Math::UP_DIR<float>, rotMatrix);
		const Vec3 target = Math::Transform(Math::FORWARD_DIR<float>, rotMatrix) + mPos;
		mViewMatrix = Math::LookAt(mPos, target, mUpDir);

		const Mat4 yawRotMatrix = Math::RotationMatrixFromEuler(0.f, mRot.Y, 0.f);
		mForwardDir = Math::Transform(Math::FORWARD_DIR<float>, yawRotMatrix);
		mRightDir = Math::Transform(Math::RIGHT_DIR<float>, yawRotMatrix);
	}
}
