// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

namespace Zeron
{
	template <typename T>
	class TVec4 {
	  public:
		T X, Y, Z, W;

		static const TVec4 ZERO;
		static const TVec4 ONE;
		static const TVec4 AXIS_X;
		static const TVec4 AXIS_Y;
		static const TVec4 AXIS_Z;
		static const TVec4 AXIS_W;

	  public:
		TVec4()
			: X(0)
			, Y(0)
			, Z(0)
			, W(0)
		{}
		TVec4(T value)
			: X(value)
			, Y(value)
			, Z(value)
			, W(value)
		{}
		TVec4(T x, T y, T z, T w)
			: X(x)
			, Y(y)
			, Z(z)
			, W(w)
		{}
		TVec4(const TVec4&) = default;
		TVec4(TVec4&&) = default;
		TVec4& operator=(const TVec4&) = default;
		TVec4& operator=(TVec4&&) = default;

		bool operator==(const TVec4& other) const { return X == other.X && Y == other.Y && Z == other.Z && W == other.W; }

		TVec4 operator+(const TVec4& other) const { return { X + other.X, Y + other.Y, Z + other.Z, W + other.W }; }

		TVec4 operator-(const TVec4& other) const { return { X - other.X, Y - other.Y, Z - other.Z, W - other.W }; }

		TVec4 operator+(T value) const { return { X + value, Y + value, Z + value, W + value }; }

		TVec4 operator-(T value) const { return { X - value, Y - value, Z - value, W - value }; }

		TVec4 operator*(T value) const { return { X * value, Y * value, Z * value, W * value }; }

		TVec4 operator/(T value) const { return value == 0 ? TVec4(0, 0, 0, 0) : TVec4{ X / value, Y / value, Z / value, W / value }; }

		TVec4& operator+=(const TVec4& other)
		{
			X += other.X;
			Y += other.Y;
			Z += other.Z;
			W += other.W;
			return *this;
		}

		TVec4& operator-=(const TVec4& other)
		{
			X -= other.X;
			Y -= other.Y;
			Z -= other.Z;
			W -= other.W;
			return *this;
		}

		TVec4& operator+=(T value)
		{
			X += value;
			Y += value;
			Z += value;
			W += value;
			return *this;
		}

		TVec4& operator-=(T value)
		{
			X -= value;
			Y -= value;
			Z -= value;
			W -= value;
			return *this;
		}

		TVec4& operator*=(T value)
		{
			X *= value;
			Y *= value;
			Z *= value;
			W *= value;
			return *this;
		}

		TVec4& operator/=(T value)
		{
			if (value != 0) {
				X /= value;
				Y /= value;
				Z /= value;
				W /= value;
			}
			else {
				X = 0, Y = 0, Z = 0, W = 0;
			}
			return *this;
		}

		TVec4 operator-() const& { return TVec4(*this) * static_cast<T>(-1); }

		TVec4 operator-() const&& { return TVec4(*this) * static_cast<T>(-1); }

		T Length() const { return sqrt((X * X) + (Y * Y) + (Z * Z) + (W * W)); }

		TVec4& Normalize()
		{
			if (const T len = Length(); len != 0) {
				*this /= len;
			}
			return *this;
		}

		static T Distance(const TVec4& from, const TVec4& to) { return sqrt(pow(to.X - from.X, 2) + pow(to.Y - from.Y, 2) + pow(to.Z - from.Z, 2) + pow(to.W - from.W, 2)); }

		static T Dot(const TVec4& v1, const TVec4& v2) { return v1.X * v2.X + v1.Y * v2.Y + v1.Z * v2.Z + v1.W * v2.W; }

		static TVec4 Normalize(const TVec4& v)
		{
			TVec4 norm(v);
			norm.Normalize();
			return norm;
		}
	};

	template <typename T>
	std::ostream& operator<<(std::ostream& os, const TVec4<T>& v)
	{
		os << "{ X:" << v.X << ", Y:" << v.Y << ", Z:" << v.Z << ", W:" << v.W << " }";
		return os;
	}

	template <class T>
	const TVec4<T> TVec4<T>::ZERO = TVec4<T>(0, 0, 0, 0);

	template <class T>
	const TVec4<T> TVec4<T>::ONE = TVec4<T>(1, 1, 1, 1);

	template <class T>
	const TVec4<T> TVec4<T>::AXIS_X = TVec4<T>(1, 0, 0, 0);

	template <class T>
	const TVec4<T> TVec4<T>::AXIS_Y = TVec4<T>(0, 1, 0, 0);

	template <class T>
	const TVec4<T> TVec4<T>::AXIS_Z = TVec4<T>(0, 0, 1, 0);

	template <class T>
	const TVec4<T> TVec4<T>::AXIS_W = TVec4<T>(0, 0, 0, 1);

	using Vec4 = TVec4<float>;
	using Vec4i = TVec4<int>;
	using Vec4d = TVec4<double>;
}