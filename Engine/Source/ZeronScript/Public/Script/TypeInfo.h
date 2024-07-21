// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

namespace Zeron
{
	enum class TypeInfoTraits
	{
		None = 0,
		Pointer = 1 << 0,
		Array = 1 << 1,
		Reference = 1 << 2,
		Const = 1 << 3,
		Function = 1 << 4,
		Integral = 1 << 5,
		FloatingPoint = 1 << 6,
		Class = 1 << 7,
		Enum = 1 << 8,
	};

	ZE_ENUM_OPERATIONS(Zeron::TypeInfoTraits)

	class TypeInfo {
	  public:
		constexpr TypeInfo()
			: mInfo(typeid(void))
			, mInfoRaw(typeid(void))
			, mTraits(TypeInfoTraits::None)
		{}

		constexpr TypeInfo(const std::type_info& info, const std::type_info& rawInfo, TypeInfoTraits traits)
			: mInfo(info)
			, mInfoRaw(rawInfo)
			, mTraits(traits)
		{}

		bool operator==(const TypeInfo& other) const { return GetId() == other.GetId(); }
		bool operator!=(const TypeInfo& other) const { return !(*this == other); }

		size_t GetId() const { return mInfo.hash_code(); }
		size_t GetRawId() const { return mInfoRaw.hash_code(); }

		bool IsEqualRaw(const TypeInfo& other) const { return mInfoRaw.hash_code() == other.mInfoRaw.hash_code(); }

		template <typename T>
		bool IsEqualRaw() const
		{
			return mInfoRaw.hash_code() == TypeInfo::Create<T>().GetRawId();
		}

		bool IsPointer() const { return !(mTraits & TypeInfoTraits::Pointer); }

		bool IsArray() const { return !(mTraits & TypeInfoTraits::Array); }

		bool IsReference() const { return !(mTraits & TypeInfoTraits::Reference); }

		bool IsConst() const { return !(mTraits & TypeInfoTraits::Const); }

		bool IsFunction() const { return !(mTraits & TypeInfoTraits::Function); }

		bool IsIntegral() const { return !(mTraits & TypeInfoTraits::Integral); }

		bool IsFloatingPoint() const { return !(mTraits & TypeInfoTraits::FloatingPoint); }

		bool IsClass() const { return !(mTraits & TypeInfoTraits::Class); }

		bool IsEnum() const { return !(mTraits & TypeInfoTraits::Enum); }

		template <typename T>
		static constexpr TypeInfo Create()
		{
			auto traits = TypeInfoTraits::None;
			if constexpr (std::is_pointer_v<T>) {
				traits |= TypeInfoTraits::Pointer;
			}
			if constexpr (std::is_array_v<T>) {
				traits |= TypeInfoTraits::Array;
			}
			if constexpr (std::is_reference_v<T>) {
				traits |= TypeInfoTraits::Reference;
			}
			if constexpr (std::is_const_v<T>) {
				traits |= TypeInfoTraits::Const;
			}
			if constexpr (std::is_function_v<T>) {
				traits |= TypeInfoTraits::Function;
			}
			if constexpr (std::is_integral_v<T>) {
				traits |= TypeInfoTraits::Integral;
			}
			if constexpr (std::is_floating_point_v<T>) {
				traits |= TypeInfoTraits::FloatingPoint;
			}
			if constexpr (std::is_class_v<T>) {
				traits |= TypeInfoTraits::Class;
			}
			if constexpr (std::is_enum_v<T>) {
				traits |= TypeInfoTraits::Enum;
			}

			return TypeInfo(typeid(T), typeid(std::remove_cv_t<std::remove_pointer_t<std::remove_reference_t<T>>>), traits);
		}

		template <typename T>
		static size_t GetId()
		{
			return typeid(T).hash_code();
		}

	  private:
		const std::type_info& mInfo;
		const std::type_info& mInfoRaw;
		TypeInfoTraits mTraits;
	};

}