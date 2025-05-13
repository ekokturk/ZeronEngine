// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

#include <Net/Http/HttpServerSession.h>

#include <asio/ip/tcp.hpp>

namespace Zeron::Net
{
	class HttpServerSentEvent;
	class HttpServerAsio;
	class HttpRequest;
	class HttpResponse;

	class HttpServerSessionAsio : public HttpServerSession {
	  public:
		HttpServerSessionAsio(HttpServerAsio& server, asio::ip::tcp::socket socket);
		~HttpServerSessionAsio();

		size_t GetId() const override;
		bool End() override;
		void SendEvent(const HttpServerSentEvent& evt, std::function<void(const std::error_code&)> callback);

	  private:
		void _sendResponse(std::shared_ptr<HttpResponse> response, bool closeSocket);
		void _readRequest();

	  private:
		asio::ip::tcp::socket mSocket;
		NotNull<HttpServerAsio> mServer;
		HttpSessionType mSessionType;
	};
}