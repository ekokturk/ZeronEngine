// Copyright (C) Eser Kokturk. All Rights Reserved.

#include <Net/WebSocket/WebSocketServer.h>

#include <Net/API/asio/WebSocketServerAsio.h>

namespace Zeron::Net
{

	std::unique_ptr<WebSocketServer> WebSocketServer::Create()
	{
		return std::make_unique<WebSocketServerAsio>();
	}
}