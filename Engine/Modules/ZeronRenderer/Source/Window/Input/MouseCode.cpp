// Copyright (C) Eser Kokturk. All Rights Reserved.

#include "MouseCode.h"

namespace Zeron {

	MouseCode::MouseCode()
        : mType(Unknown)
	{
	}

    MouseCode::MouseCode(Type button)
        : mType(button)
	{
	}

	MouseCode::MouseCode(int button)
        : mType(static_cast<Type>(button))
	{
	}

	const char* MouseCode::ToString() const
    {
        switch (mType) {
            case LeftButton:	return "Mouse Button Left";
            case RightButton:	return "Mouse Button Right";
            case MiddleButton:	return "Mouse Button Middle";
            case ButtonX1:		return "Mouse Button X1";
            case ButtonX2:		return "Mouse Button X2";
            case Button5:		return "Mouse Button 5";
            case Button6:		return "Mouse Button 6";
            case Button7:		return "Mouse Button 7";
            case Unknown:		return "Unknown Mouse Button";
            default:			return "Invalid Mouse Button";
        }
    }
}