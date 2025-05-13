// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

namespace Zeron::Net
{
	class HttpServerSentEvent;
	class HttpResponse;
	class HttpRequest;

	class HttpServer {
	  public:
		using RequestCallback = std::function<std::optional<HttpResponse>(size_t sessionId, const HttpRequest&)>;

		static std::unique_ptr<HttpServer> Create();

		virtual ~HttpServer() = default;

		virtual bool Start(size_t port, RequestCallback requestCallback) = 0;
		virtual void Stop() = 0;
		virtual void Update() = 0;

		virtual bool HasSession(size_t connId) const = 0;
		virtual void SendEvent(size_t connId, const HttpServerSentEvent& evt, std::function<void(const std::error_code&)> callback = nullptr) = 0;
	};
}