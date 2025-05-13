// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

#include <asio/ip/tcp.hpp>
#include <Net/Http/HttpServer.h>

namespace Zeron::Net
{
	class HttpServerSentEvent;
	class HttpRequest;
	class HttpResponse;
	class HttpServerSessionAsio;

	class HttpServerAsio : public HttpServer {
	  public:
		HttpServerAsio();
		~HttpServerAsio() override;

		bool Start(size_t port, RequestCallback callback) override;
		void Stop() override;
		void Update() override;
		bool HasSession(size_t connId) const override;

		void SendEvent(size_t connId, const HttpServerSentEvent& evt, std::function<void(const std::error_code&)> callback) override;

		// Asio
		std::optional<HttpResponse> OnRequest(size_t connId, std::shared_ptr<HttpRequest> request);
		void OnClose(size_t connId);

	  private:
		void _acceptConnection();

	  private:
		std::unique_ptr<asio::io_context> mIO;
		asio::ip::tcp::acceptor mAcceptor;
		RequestCallback mRequestCallback;

		std::unordered_map<size_t, std::unique_ptr<HttpServerSessionAsio>> mConnections;
	};
}