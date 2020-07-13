// Copyright (C) 2020, Eser Kokturk. All Rights Reserved.

#pragma once

/*
 * Vector 2 implementation
 */
namespace ZeronEngine
{
	template<typename T>
	struct TVector2
	{
		T X;
		T Y;

		
		TVector2(): X(0),Y(0) {}
		TVector2(T x, T y): X(x), Y(y) {}

		bool operator==(const TVector2& other) const
		{
			return X == other.X && Y == other.Y;
		}
		
	};

	using Vector2  = TVector2<float>;
	using Vector2i = TVector2<int>;
	using Vector2d = TVector2<double>;
}