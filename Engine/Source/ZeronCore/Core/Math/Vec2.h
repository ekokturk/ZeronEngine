// Copyright (C) 2020, Eser Kokturk. All Rights Reserved.

#pragma once

namespace Zeron
{
	template<typename T>
	class TVec2 {
	public:
		T X, Y;

		static const TVec2 ZERO;
		static const TVec2 ONE;
		static const TVec2 AXIS_X;
		static const TVec2 AXIS_Y;
	
	public:
		TVec2() : X(0), Y(0) {}
		TVec2(T value) : X(value), Y(value) {}
		TVec2(T x, T y) : X(x), Y(y) {}
		TVec2(const TVec2&) = default;
		TVec2(TVec2&&) = default;
		TVec2& operator=(const TVec2&) = default;
		TVec2& operator=(TVec2&&) = default;

		bool operator==(const TVec2& other) const {
			return X == other.X && Y == other.Y;
		}

		TVec2 operator+(const TVec2& other) const {
			return { X + other.X, Y + other.Y };
		}

		TVec2 operator-(const TVec2& other) const {
			return { X - other.X, Y - other.Y };
		}

		TVec2 operator+(T value) const {
			return { X + value, Y + value };
		}

		TVec2 operator-(T value) const {
			return { X - value, Y - value };
		}

		TVec2 operator*(T value) const {
			return { X * value, Y * value };
		}

		TVec2 operator/(T value) const {
			return value == 0 ? TVec3(0, 0) : { X / value, Y / value };
		}

		TVec2& operator+=(const TVec2& other) {
			X += other.X; Y += other.Y;
			return *this;
		}

		TVec2& operator-=(const TVec2& other) {
			X -= other.X; Y -= other.Y;
			return *this;
		}

		TVec2& operator+=(T value) {
			X += value; Y += value;
			return *this;
		}

		TVec2& operator-=(T value) {
			X -= value; Y -= value;
			return *this;
		}

		TVec2& operator*=(T value) {
			X *= value; Y *= value;
			return *this;
		}

		TVec2& operator/=(T value) {
			if (value != 0) {
				X += value; Y += value;
			}
			else {
				X = 0, Y = 0;
			}
			return *this;
		}

		TVec2 operator-() const & {
			return TVec2(*this) * static_cast<T>(-1);
		}

		TVec2 operator-() const && {
			return TVec2(*this) * static_cast<T>(-1);
		}

		T Length() const
		{
			return sqrt((X * X) + (Y * Y));
		}

		void Normalize()
		{
			if (const T len = Length(); len != 0) {
				*this /= len;
			}
		}

		static T Distance(const TVec2& from, const TVec2& to)
		{
			return sqrt(pow(to.X - from.X, 2) + pow(to.Y - from.Y, 2));
		}

		static T Dot(const TVec2& v1, const TVec2& v2)
		{
			return v1.X * v2.X + v1.Y * v2.Y;
		}

	};

	template<typename T>
	std::ostream& operator<<(std::ostream& os, const TVec2<T>& v)
	{
		os << "{ X:" << v.X << ", Y:" << v.Y << " }";
		return os;
	}

	template<class T> const TVec2<T> TVec2<T>::ZERO		= TVec2<T>(0, 0);
	template<class T> const TVec2<T> TVec2<T>::ONE		= TVec2<T>(1, 1);
	template<class T> const TVec2<T> TVec2<T>::AXIS_X	= TVec2<T>(1, 0);
	template<class T> const TVec2<T> TVec2<T>::AXIS_Y	= TVec2<T>(0, 1);

	using Vec2	= TVec2<float>;
	using Vec2i = TVec2<int>;
	using Vec2d = TVec2<double>;
}
