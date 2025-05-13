// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

#include <Net/WebSocket/WebSocketConnectionState.h>
#include <Net/WebSocket/WebSocketMessage.h>
#include <Net/WebSocket/WebSocketSession.h>

namespace Zeron::Net
{
	class HttpUrl;

	class WebSocketClient : public WebSocketSession {
	  public:
		static std::unique_ptr<WebSocketClient> Create();
		virtual ~WebSocketClient() = default;

		virtual void Connect(const HttpUrl& url, WebSocketMessageCallback callback) = 0;
		virtual void Update() = 0;
		virtual WebSocketConnectionState GetConnectionState() const = 0;
	};
}