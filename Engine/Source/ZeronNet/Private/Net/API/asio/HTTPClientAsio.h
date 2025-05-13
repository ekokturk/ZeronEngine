// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

#include <asio/io_context.hpp>
#include <asio/ip/tcp.hpp>
#include <Net/Http/HttpClient.h>
#include <Net/Http/HttpResponse.h>
#include <Net/Http/HttpServerSentEvent.h>


namespace Zeron::Net
{
	class HttpClientSessionAsio;

	class HttpClientAsio : public HttpClient {
	  public:
		HttpClientAsio(const HttpClientConfig& config);
		~HttpClientAsio() override;

		void Update() override;

		std::weak_ptr<HttpClientSession> CreateSession(const std::string& url) override;
		void SendRequest(HttpRequest request, HttpResponseCallback callback) override;
		void SendEventRequest(const HttpUrl& url, HttpServerSentEventCallback callback) override;

		asio::io_context& GetIO();

	  private:
		std::unique_ptr<asio::io_context> mIO;
		asio::ip::tcp::resolver mResolver;
		asio::ip::tcp::socket mSocket;
		std::vector<std::shared_ptr<HttpClientSessionAsio>> mSessions;
	};
}