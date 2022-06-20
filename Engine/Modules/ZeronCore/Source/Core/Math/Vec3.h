// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

namespace Zeron
{
	template<typename T>
	class TVec3 {
	public:
		T X, Y, Z;
		
		static const TVec3 ZERO;
		static const TVec3 ONE;
		static const TVec3 AXIS_X;
		static const TVec3 AXIS_Y;
		static const TVec3 AXIS_Z;

	public:
		TVec3()							: X(0), Y(0), Z(0) {}
		TVec3(T value)					: X(value), Y(value), Z(value) {}
		TVec3(T x, T y, T z)			: X(x), Y(y), Z(z) {}
		TVec3(const TVec3&)				= default;
		TVec3(TVec3&&)					= default;
		TVec3& operator=(const TVec3&)	= default;
		TVec3& operator=(TVec3&&)		= default;

		bool operator==(const TVec3& other) const {
			return X == other.X && Y == other.Y && Z == other.Z;
		}

		bool operator!=(const TVec3& other) const {
			return X != other.X || Y != other.Y || Z != other.Z;
		}

		TVec3 operator+(const TVec3& other) const {
			return { X + other.X, Y + other.Y, Z + other.Z };
		}

		TVec3 operator-(const TVec3& other) const {
			return { X - other.X, Y - other.Y, Z - other.Z };
		}

		TVec3 operator+(T value) const {
			return { X + value, Y + value, Z + value };
		}

		TVec3 operator-(T value) const {
			return { X - value, Y - value, Z - value };
		}

		TVec3 operator*(T value) const {
			return { X * value, Y * value, Z * value };
		}

		TVec3 operator/(T value) const {
			return value == 0 ? TVec3(0, 0, 0) : TVec3{ X / value, Y / value, Z / value };
		}

		TVec3& operator+=(const TVec3& other) {
			X += other.X; Y += other.Y; Z += other.Z;
			return *this;
		}

		TVec3& operator-=(const TVec3& other) {
			X -= other.X; Y -= other.Y; Z -= other.Z;
			return *this;
		}

		TVec3& operator+=(T value) {
			X += value; Y += value; Z += value;
			return *this;
		}

		TVec3& operator-=(T value) {
			X -= value; Y -= value; Z -= value;
			return *this;
		}

		TVec3& operator*=(T value) {
			X *= value; Y *= value; Z *= value;
			return *this;
		}

		TVec3& operator/=(T value) {
			if (value != 0) {
				X += value; Y += value; Z += value;
			}
			else {
				X = 0, Y = 0, Z = 0;
			}
			return *this;
		}

		TVec3 operator-() const & {
			return TVec3(*this) * static_cast<T>(-1);
		}

		TVec3 operator-() const && {
			return TVec3(*this) * static_cast<T>(-1);
		}

		T Length() const
		{
			return sqrt((X * X) + (Y * Y) + (Z * Z));
		}

		void Normalize()
		{
			if (const T len = Length(); len != 0) {
				*this /= len;
			}
		}

		static T Distance(const TVec3& from, const TVec3& to)
		{
			return sqrt(pow(to.X - from.X, 2) + pow(to.Y - from.Y, 2) + pow(to.Z - from.Z, 2));
		}

		static T Dot(const TVec3& v1, const TVec3& v2)
		{
			return v1.X * v2.X + v1.Y * v2.Y + v1.Z * v2.Z;
		}

		static TVec3 Cross(const TVec3& v1, const TVec3& v2)
		{
			return { v1.Y * v2.Z - v1.Z * v2.Y,
					 v1.Z * v2.X - v1.X * v2.Z,
					 v1.X * v2.Y - v1.Y * v2.X };
		}
	};

	template<typename T>
	std::ostream& operator<<(std::ostream& os, const TVec3<T>& v)
	{
		os << "{ X:" << v.X << ", Y:" << v.Y << ", Z:" << v.Z << " }";
		return os;
	}

	template<class T> const TVec3<T> TVec3<T>::ZERO		= TVec3<T>(0, 0, 0);
	template<class T> const TVec3<T> TVec3<T>::ONE		= TVec3<T>(1, 1, 1);
	template<class T> const TVec3<T> TVec3<T>::AXIS_X	= TVec3<T>(1, 0, 0);
	template<class T> const TVec3<T> TVec3<T>::AXIS_Y	= TVec3<T>(0, 1, 0);
	template<class T> const TVec3<T> TVec3<T>::AXIS_Z	= TVec3<T>(0, 0, 1);

	using Vec3	= TVec3<float>;
	using Vec3i = TVec3<int>;
	using Vec3d = TVec3<double>;
}
