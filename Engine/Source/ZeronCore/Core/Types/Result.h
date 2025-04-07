// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

namespace Zeron
{
	template <typename E>
	class ErrorResult {
	  public:
		ErrorResult(E&& value)
			: mError(std::move(value))
		{}

		E&& ErrorValue()
		{
			E&& val = std::move(mError);
			mError = E();
			return std::move(val);
		}

	  private:
		E mError;
	};

	template <typename V, typename E>
	class Result {
		using ErrorType = ErrorResult<E>;

	  public:
		Result() = default;

		template <typename T = V>
			requires(!std::is_void_v<T>)
		Result(T&& value)
			: mValue(std::forward<T>(value))
		{}

		Result(E&& error)
			requires(!std::is_same_v<V, E>)
			: mError(std::move(error))
		{}

		Result(ErrorType&& error)
			: mError(std::move(error.ErrorValue()))
		{}

		Result(Result&& other) noexcept
			: mError(std::move(other.mError))
		{
			if constexpr (!std::is_void_v<V>) {
				mValue = std::move(other.mValue);
			}
		}

		Result& operator=(Result&& other) noexcept
		{
			if constexpr (!std::is_void_v<V>) {
				mValue = std::move(other.mValue);
			}
			mError = std::move(other.mError);
			return *this;
		}

		Result& operator=(ErrorType&& other) noexcept
		{
			if constexpr (!std::is_void_v<V>) {
				mValue = std::nullopt;
			}
			mError = std::move(other.ErrorValue());
			return *this;
		}

		Result(const Result&) = delete;
		Result& operator=(const Result&) = delete;

		~Result() = default;

		template <typename T = V>
			requires(!std::is_void_v<T>)
		T&& Value()
		{
			T&& val = std::move(*mValue);
			return std::move(val);
		}

		template <typename T = V>
			requires(!std::is_void_v<T>)
		const T& Value() const
		{
			return *mValue;
		}
		const E& GetError() const { return *mError; }

		operator bool() const { return !HasError(); }

		template <typename T = V>
			requires(!std::is_void_v<T>) bool
		HasValue() const
		{
			return mValue.has_value();
		}
		bool HasError() const { return mError.has_value(); }

	  private:
		std::conditional_t<std::is_void_v<V>, std::monostate, std::optional<V>> mValue;
		std::optional<E> mError;
	};
}