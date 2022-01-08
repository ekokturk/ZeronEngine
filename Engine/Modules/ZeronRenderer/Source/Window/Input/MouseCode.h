// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

namespace Zeron
{
	struct MouseCode
	{
		enum Type : uint8_t {
			LeftButton = 0,
			RightButton,
			MiddleButton,
			ButtonX1,
			ButtonX2,
			Button5,
			Button6,
			Button7,
			AxisX,
			AxisY,
			AxisScroll,
			COUNT,
			Unknown
		};

		MouseCode();
		MouseCode(Type button);
		MouseCode(int button);

		operator Type() const							{ return mType; }
		operator const int() const						{ return static_cast<int>(mType); }
		bool operator ==(const MouseCode& other) const	{ return mType == other.mType; }
		bool operator !=(const MouseCode& other) const	{ return mType != other.mType; }
		bool operator ==(const Type& other) const		{ return mType == other; }
		bool operator !=(const Type& other) const		{ return mType != other; }
		
		MouseCode& operator =(const MouseCode& other) = default;
		
		// Get human readable string name of the mouse code
		const char* ToString() const;

	private:
		Type mType;
	};
}
