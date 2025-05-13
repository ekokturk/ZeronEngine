// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

#include <Net/Http/HttpRequest.h>
#include <Net/Http/HttpResponse.h>
#include <Net/Http/HttpServerSentEvent.h>

namespace Zeron::Net
{
	class HttpClientSession;

	struct HttpClientConfig {
		Time::Milliseconds mConnTimeout{ 5000 };
		Time::Milliseconds mResponseTimeout{ 15000 };
	};

	class HttpClient {
	  public:
		static std::unique_ptr<HttpClient> Create(const HttpClientConfig& config = {});
		virtual ~HttpClient() = default;

		virtual std::weak_ptr<HttpClientSession> CreateSession(const std::string& url) = 0;
		virtual void SendRequest(HttpRequest request, HttpResponseCallback callback) = 0;
		virtual void SendEventRequest(const HttpUrl& url, HttpServerSentEventCallback callback) = 0;
		virtual void Update() = 0;

		const HttpClientConfig& GetConfig() const { return mConfig; }

	  protected:
		HttpClient(const HttpClientConfig& config)
			: mConfig(config)
		{}

		const HttpClientConfig mConfig;
	};
}