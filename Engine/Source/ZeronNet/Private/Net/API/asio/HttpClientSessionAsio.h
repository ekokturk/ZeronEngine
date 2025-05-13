// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

#include <asio/io_context.hpp>
#include <asio/ip/tcp.hpp>

#include <Net/Http/HttpClientSession.h>
#include <Net/Http/HttpRequest.h>
#include <Net/Http/HttpResponse.h>
#include <Net/Http/HttpServerSentEvent.h>

namespace Zeron::Net
{
	class HttpResponse;
	class HttpClientAsio;

	class HttpClientSessionAsio : public HttpClientSession {
	  public:
		HttpClientSessionAsio(HttpClientAsio& client, HttpSessionType sessionType);
		~HttpClientSessionAsio();

		size_t GetId() const override;
		void Start() override;
		bool End() override;
		bool IsAlive() const override;
		Zeron::Time::TimePoint GetLastStartRequest() const;

		void SendRequest(HttpRequest request, HttpResponseCallback callback);
		void SendEventRequest(const HttpUrl& url, HttpServerSentEventCallback callback);


	  private:
		void _startConnection(const std::string& host, size_t port, std::function<void(const std::error_code&)> callback);

		// REST
		void _tryConnectAndSendRequest(std::shared_ptr<HttpRequest> request, HttpResponseCallback callback);
		void _sendRequest(std::shared_ptr<HttpRequest> request, HttpResponseCallback callback);
		void _readResponse(HttpResponseCallback callback);

		// SSE
		void _sendSSE(std::shared_ptr<HttpRequest> request, HttpServerSentEventCallback callback);
		void _readSSE(HttpServerSentEventCallback callback);

	  private:
		NotNull<HttpClientAsio> mClient;
		asio::ip::tcp::socket mSocket;
		asio::ip::tcp::resolver mResolver;
		Time::TimePoint mLastStartRequest;
		HttpSessionType mSessionType;
	};
}