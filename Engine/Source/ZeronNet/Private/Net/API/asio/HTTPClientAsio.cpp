// Copyright (C) Eser Kokturk. All Rights Reserved.

#include <Net/API/asio/HTTPClientAsio.h>

#include <asio/connect.hpp>
#include <asio/ip/tcp.hpp>
#include <Net/API/asio/HttpClientSessionAsio.h>

namespace Zeron::Net
{
	HttpClientAsio::HttpClientAsio(const HttpClientConfig& config)
		: HttpClient(config)
		, mIO(std::make_unique<asio::io_context>())
		, mResolver(*mIO)
		, mSocket(*mIO)
	{}

	HttpClientAsio::~HttpClientAsio()
	{
		mIO->stop();
	}

	void HttpClientAsio::Update()
	{
		mIO->poll();

		// Clean up dead connection
		mSessions.erase(
			std::ranges::remove_if(
				mSessions,
				[](const std::shared_ptr<HttpClientSessionAsio> conn) {
					return !conn->IsAlive() && Time::CalculateElapsedTime<float>(Time::Now(), conn->GetLastStartRequest()) > 1000;
				}
			).begin(),
			mSessions.end()
		);
	}

	std::weak_ptr<HttpClientSession> HttpClientAsio::CreateSession(const std::string& url)
	{
		auto session = std::make_shared<HttpClientSessionAsio>(*this, HttpSessionType::Rest);
		mSessions.push_back(session);
		session->Start();
		return session;
	}

	void HttpClientAsio::SendRequest(HttpRequest request, HttpResponseCallback callback)
	{
		auto session = std::make_shared<HttpClientSessionAsio>(*this, HttpSessionType::Rest);
		mSessions.push_back(session);
		session->SendRequest(std::move(request), std::move(callback));
	}

	void HttpClientAsio::SendEventRequest(const HttpUrl& url, HttpServerSentEventCallback callback)
	{
		auto session = std::make_shared<HttpClientSessionAsio>(*this, HttpSessionType::SSE);
		mSessions.push_back(session);
		session->SendEventRequest(url, std::move(callback));
	}

	asio::io_context& HttpClientAsio::GetIO()
	{
		return *mIO;
	}

}