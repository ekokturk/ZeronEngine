// Copyright (C) 2020, Eser Kokturk. All Rights Reserved.

#pragma once

namespace ZeronEngine
{

	/*
	 * Custom hasher that can be used during compile time
	 * Alternative to std::hash
	 */
	
	template <typename T> size_t constexpr Hash(T value);

	template <>
	constexpr size_t Hash<const char*>(const char* input)
	{
		// String hashing
		// https://stackoverflow.com/questions/2111667/compile-time-string-hashing/2112111#2112111
		
		return *input ? static_cast<size_t>(*input) + 33 * Hash(input + 1) : 5381;
	}

}
