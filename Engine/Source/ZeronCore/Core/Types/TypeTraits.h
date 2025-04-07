// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

namespace Zeron::TypeTraits
{
	template <typename T, typename... Args>
	struct FirstType {
		using type = T;
	};

	// Variant
	template <class... Ts>
	struct Visitor : Ts... {
		using Ts::operator()...;
	};

	template <class... Ts>
	Visitor(Ts...) -> Visitor<Ts...>;

	template <typename T>
	inline constexpr bool false_v = false;

	// clang-format off

	template <typename T>
	concept ContainerType = 
    requires(T t) {
		typename T::value_type;
        requires std::input_iterator<decltype(std::begin(t))>;
        requires std::input_iterator<decltype(std::end(t))>;
    };

	// clang-format on

}