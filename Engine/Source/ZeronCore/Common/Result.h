// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

namespace Zeron
{
	template <typename E>
	class ErrorResult {
	public:
		ErrorResult(E&& value)
			: mError(std::move(value))
		{
		}

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
		Result(V&& value)
			: mValue(std::move(value))
		{
		}

		template <std::enable_if<!std::is_same_v<V, E>>>
		Result(E&& error)
			: mError(std::move(error))
		{
		}

		Result(ErrorType&& error)
			: mError(std::move(error.ErrorValue()))
		{
		}

		Result(Result&& other) noexcept
			: mValue(std::move(other.mValue))
			, mError(std::move(other.mError))
		{
		}

		Result& operator=(Result&& other) noexcept
		{
			mValue = std::move(other.mValue);
			mError = std::move(other.mError);
			return *this;
		}

		Result& operator=(ErrorType&& other) noexcept
		{
			mValue = std::nullopt;
			mError = std::move(other.ErrorValue());
			return *this;
		}

		Result(const Result&) = delete;
		Result& operator=(const Result&) = delete;

		~Result() = default;

		V&& Value()
		{
			V&& val = std::move(*mValue);
			return std::move(val);
		}

		const V& Value() const { return *mValue; }
		const E& GetError() const { return *mError; }

		operator bool() const { return !mError; }
		bool HasValue() const { return mValue.has_value(); }
		bool HasError() const { return mError.has_value(); }

	private:
		std::optional<V> mValue;
		std::optional<E> mError;
	};
}
