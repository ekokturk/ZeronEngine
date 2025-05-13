// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

namespace Zeron::Net
{
	enum class WebSocketConnectionState
	{
		Idle = 0,
		Connecting,
		Connected,
	};
}