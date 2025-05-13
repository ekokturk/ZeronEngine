// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

#include <Net/WebSocket/WebSocketClient.h>

#include <asio/io_context.hpp>
#include <asio/ip/tcp.hpp>
#include <Net/Http/HttpRequest.h>

namespace Zeron::Net
{
	class HttpUrl;
	class HttpClientSessionAsio;

	class WebSocketClientAsio : public WebSocketClient {
	  public:
		WebSocketClientAsio();
		~WebSocketClientAsio() override;

		void Update() override;
		void Connect(const HttpUrl& url, WebSocketMessageCallback callback) override;
		void SendText(const std::string& message) override;
		void SendBinary(const ByteBuffer& message) override;
		void End() override;
		size_t GetId() const override;
		WebSocketConnectionState GetConnectionState() const override;

		asio::io_context& GetIO();

	  private:
		void _handleHandshake(const HttpRequest& request);

	  private:
		std::unique_ptr<asio::io_context> mIO;

		struct ConnectionState {
			asio::ip::tcp::socket mSocket;
			std::string mKey;
		};
		std::unique_ptr<ConnectionState> mConnectionState;

		std::unique_ptr<WebSocketMessageHandler> mMessageHandler;

		std::unique_ptr<WebSocketSession> mSession;
		asio::ip::tcp::resolver mResolver;
	};
}