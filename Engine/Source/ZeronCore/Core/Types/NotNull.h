// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

template <typename T>
class NotNull {
  public:
	explicit NotNull(T* ptr)
		: mPtr(ptr)
	{
		static_assert(!std::is_pointer_v<T>, "T must be a pointer type");
		_checkPtr();
	}

	T& operator*() const
	{
		_checkPtr();
		return *mPtr;
	}

	T* operator->() const
	{
		_checkPtr();
		return mPtr;
	}

	operator T*() const
	{
		_checkPtr();
		return mPtr;
	}

  private:
	void _checkPtr() const { ZE_ASSERT(mPtr != nullptr, "Expected object to be not null"); }

	T* mPtr;
};