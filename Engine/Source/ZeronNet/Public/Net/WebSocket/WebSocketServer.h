// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once
#include <Net/WebSocket/WebSocketMessage.h>

namespace Zeron::Net
{
	class WebSocketServer {
	  public:
		static std::unique_ptr<WebSocketServer> Create();

		virtual ~WebSocketServer() = default;

		virtual bool Start(size_t port, WebSocketMessageCallback callback) = 0;
		virtual void Stop() = 0;
		virtual void Update() = 0;
		virtual bool IsRunning() const = 0;

		virtual std::vector<size_t> GetSessionList() const = 0;
		virtual bool HasSession(size_t connId) const = 0;
		virtual bool EndSession(size_t connId) = 0;
		virtual void SendText(const std::string& message, size_t connId = 0) = 0;
		virtual void SendBinary(const ByteBuffer& message, size_t connId = 0) = 0;
	};
}