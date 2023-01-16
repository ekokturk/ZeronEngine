// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

namespace Zeron::TypeTraits {

	template<typename T, typename ...Args> struct FirstType { using type = T; };

    // Variant
	template<class... Ts> struct Visitor : Ts... { using Ts::operator()...; };
	template<class... Ts> Visitor(Ts...) -> Visitor<Ts...>;

}