// Copyright (C) Eser Kokturk. All Rights Reserved.

#include <Net/API/asio/WebSocketClientAsio.h>

#include <Net/WebSocket/WebSocketClient.h>

namespace Zeron::Net
{

	std::unique_ptr<WebSocketClient> WebSocketClient::Create()
	{
		return std::make_unique<WebSocketClientAsio>();
	}
}