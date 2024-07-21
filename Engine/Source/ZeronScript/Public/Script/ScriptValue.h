// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once
#include <Script/TypeInfo.h>

namespace Zeron::Script
{
	class Value {
	  public:
		Value()
			: mData(std::any())
		{}

		template <typename T>
		Value(const T& value)
			: mData(value)
			, mInfo(TypeInfo::Create<T>())
		{}

		template <typename T>
		void Set(const T& value)
		{
			mData = value;
			mInfo = TypeInfo::Create<T>();
		}

		template <typename T>
		T Get() const
		{
			// TODO: Return without throwing, possibly std::optional
			return std::any_cast<T>(mData);
		}

		template <typename T>
		bool IsType() const
		{
			return mData.type() == typeid(T);
		}

		const TypeInfo& GetTypeInfo() const { return mInfo; }

		bool HasValue() const { return mData.has_value(); }

	  private:
		std::any mData;
		TypeInfo mInfo;
	};

	using ValueList = std::vector<Value>;

}
