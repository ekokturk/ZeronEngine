// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

namespace Zeron
{
	class Entity {
	public:
		using Id = uint32_t;
		using Version = uint32_t;
		static constexpr Id INVALID_ID = std::numeric_limits<Id>::max();
		static constexpr Version INVALID_VERSION = std::numeric_limits<Version>::max();

		Entity(Id id, Version version = 0u) : mId(id), mVersion(version) {}

		bool operator==(const Entity& other) const {
			return mId == other.mId && mVersion == other.mVersion;
		}
		bool operator!=(const Entity& other) const {
			return !(other == *this);
		}

		Id GetId() const				{ return mId; }
		Version GetVersion() const		{ return mVersion; }
		bool isValid() const			{ return mId != INVALID_ID && mVersion != INVALID_VERSION; }
		bool hasValidId() const			{ return mId != INVALID_ID; }
		bool hasValidVersion() const	{ return mVersion != INVALID_VERSION; }

		void IncrementVersion() {
			mVersion++;
		}

	private:
		Id mId = 0;
		Version mVersion = 0;
	};
}
