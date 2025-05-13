// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

namespace Zeron
{
	class Random {
	  public:
		Random()
			: mSeed(static_cast<unsigned int>(std::chrono::high_resolution_clock::now().time_since_epoch().count()))
			, mGenerator(mSeed)
		{}

		explicit Random(unsigned int seed)
			: mSeed(seed)
			, mGenerator(seed)
		{}

		unsigned int GetSeed() const { return mSeed; }

		std::byte GetByte()
		{
			std::uniform_int_distribution<int> dist(0, 255);
			return static_cast<std::byte>(static_cast<uint8_t>(dist(mGenerator)));
		}

		int GetInt(int min, int max)
		{
			std::uniform_int_distribution<int> dist(min, max);
			return dist(mGenerator);
		}

		float GetFloat(float min, float max)
		{
			std::uniform_real_distribution<float> dist(min, max);
			return dist(mGenerator);
		}

		double GetDouble(double min, double max)
		{
			std::uniform_real_distribution<double> dist(min, max);
			return dist(mGenerator);
		}

	  private:
		unsigned int mSeed;
		std::mt19937 mGenerator;
	};
}