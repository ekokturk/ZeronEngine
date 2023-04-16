// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

#include <chrono>

namespace Zeron::Time
{
	using Clock = std::conditional_t<std::chrono::high_resolution_clock::is_steady, std::chrono::high_resolution_clock, std::chrono::steady_clock>;
	using TimePoint = Clock::time_point;

	using Seconds = std::chrono::duration<long double>;
	using Milliseconds = std::chrono::duration<long double, std::milli>;
	using Microseconds = std::chrono::duration<long double, std::micro>;
	using Nanoseconds = std::chrono::duration<long double, std::nano>;
	template <typename T> concept SecondsUnit = std::is_same_v<T, Seconds> || std::is_same_v<T, Milliseconds> || std::is_same_v<T, Microseconds> ||
		std::is_same_v<T, Nanoseconds>;

	using CalendarDate = std::chrono::year_month_day;
	using TimeOfDay = std::chrono::hh_mm_ss<Microseconds>;

	struct TimeStamp {
		int CountSeconds() const { return static_cast<int>(mTime.seconds().count()); }

		int CountMinutes() const { return mTime.minutes().count(); }

		int CountHours() const { return mTime.hours().count(); }

		CalendarDate mDate{ floor<std::chrono::days>(std::chrono::system_clock::time_point()) };
		TimeOfDay mTime{ Microseconds::zero() };
	};

	inline TimeStamp GetLocalTime()
	{
		const auto now = std::chrono::system_clock::now();
#if ZE_COMPILER_CLANG
		// Clang does not support std::chrono::zoned_time for C++20 yet
		const auto nowTimeT = std::chrono::system_clock::to_time_t(now);
		std::tm buf;
		localtime_r(&nowTimeT, &buf);
		const CalendarDate ymd{ std::chrono::year(buf.tm_year + 1900), std::chrono::month(buf.tm_mon + 1), std::chrono::day(buf.tm_mday) };
		const Milliseconds timeInMs = std::chrono::duration_cast<Milliseconds>(Seconds(buf.tm_sec)) +
			std::chrono::duration_cast<Milliseconds>(std::chrono::minutes(buf.tm_min)) + std::chrono::duration_cast<Milliseconds>(std::chrono::hours(buf.tm_hour));
		const TimeOfDay tod{ timeInMs };
#else
		const auto time = std::chrono::zoned_time{ std::chrono::current_zone(), now }.get_local_time();
		const auto dayPoint = floor<std::chrono::days>(time);
		const CalendarDate ymd{ dayPoint };
		const TimeOfDay tod{ std::chrono::duration_cast<Microseconds>(time - dayPoint) };
#endif
		return TimeStamp{ ymd, tod };
	}

	inline TimePoint Now()
	{
		return Clock::now();
	}

	template <typename T, SecondsUnit Precision = Milliseconds>
	T CalculateElapsedTime(TimePoint now, TimePoint then)
	{
		return static_cast<T>(std::chrono::duration_cast<Precision>(now - then).count());
	}

	template <typename T = float, SecondsUnit Precision = Milliseconds>
	class Timer {
		static_assert(std::is_arithmetic_v<T>);

	  public:
		void Reset() { mTimePoint = Now(); }

		TimePoint GetStartTime() const { return mTimePoint; }

		T GetElapsedTime() const { return CalculateElapsedTime<T, Precision>(Now(), mTimePoint); }

		bool hasTimeElapsed(T expectedTime) { return GetElapsedTime() > expectedTime; }

	  private:
		TimePoint mTimePoint = Now();
	};


	class TickTimer {
	  public:
		using TickType = uint64_t;

		TickTimer(Milliseconds tickRate, Milliseconds maxDeltaTime = Milliseconds(250))
			: mTickCount(0)
			, mTickRate(tickRate)
			, mAccumulated(0)
			, mMaxDeltaTime(maxDeltaTime)
			, mLastDeltaTime(0)
			, mLastTick(Now())
			, mAlpha(0)
		{}

		TickType Tick()
		{
			const TickType oldTickCount = mTickCount;
			const TimePoint now = Now();
			mLastDeltaTime = std::chrono::duration_cast<Milliseconds>(now - mLastTick);
			mLastTick = now;

			const Milliseconds deltaTime = mLastDeltaTime > mMaxDeltaTime ? mMaxDeltaTime : mLastDeltaTime;
			mAccumulated += deltaTime;

			// TODO: Timescale
			while (mAccumulated >= mTickRate) {
				++mTickCount;
				mAccumulated -= mTickRate;
			}
			mAlpha = static_cast<double>(mAccumulated / mTickRate);

			return mTickCount - oldTickCount;
		}

		TickType GetTickCount() const { return mTickCount; }

		Milliseconds GetTickRate() const { return mTickRate; }

		Milliseconds LastDeltaTime() const { return mLastDeltaTime; }

		double GetAlpha() const { return mAlpha; }

	  private:
		TickType mTickCount;
		Milliseconds mTickRate;
		Milliseconds mAccumulated;
		Milliseconds mMaxDeltaTime;
		Milliseconds mLastDeltaTime;
		TimePoint mLastTick;
		double mAlpha;
	};
}