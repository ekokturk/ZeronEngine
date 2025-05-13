// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

#include <asio/ip/tcp.hpp>
#include <Net/Websocket/WebSocketServer.h>

namespace Zeron::Net
{
	class WebSocketSession;

	class WebSocketServerAsio : public WebSocketServer {
	  public:
		WebSocketServerAsio();
		~WebSocketServerAsio() override;

		bool Start(size_t port, WebSocketMessageCallback callback) override;
		void Stop() override;
		void Update() override;
		bool IsRunning() const override;

		std::vector<size_t> GetSessionList() const override;
		bool HasSession(size_t connId) const override;
		bool EndSession(size_t connId) override;
		void SendText(const std::string& message, size_t connId) override;
		void SendBinary(const ByteBuffer& message, size_t connId) override;

	  private:
		void _acceptConnection();
		void _handleHandshake(std::shared_ptr<asio::ip::tcp::socket> socket);
		WebSocketSession* _getSession(size_t connId) const;

	  private:
		std::unique_ptr<asio::io_context> mIO;
		asio::ip::tcp::acceptor mAcceptor;
		std::unique_ptr<WebSocketMessageHandler> mMessageHandler;

		std::unordered_map<size_t, std::unique_ptr<WebSocketSession>> mConnections;
	};
}