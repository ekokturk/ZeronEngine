// Copyright (C) Eser Kokturk. All Rights Reserved.

#include <Net/API/asio/HttpClientSessionAsio.h>

#include <asio/connect.hpp>
#include <asio/ip/tcp.hpp>
#include <asio/write.hpp>
#include <Net/API/asio/AsioUtils.h>
#include <Net/API/asio/HttpClientAsio.h>
#include <Net/Http/HttpResponse.h>
#include <Net/Http/HttpServerSentEvent.h>

namespace Zeron::Net
{
	HttpClientSessionAsio::HttpClientSessionAsio(HttpClientAsio& client, HttpSessionType sessionType)
		: mClient(&client)
		, mSocket(mClient->GetIO())
		, mResolver(mClient->GetIO())
		, mSessionType(sessionType)
	{}

	HttpClientSessionAsio::~HttpClientSessionAsio()
	{
		mSocket.close();
	}

	size_t HttpClientSessionAsio::GetId() const
	{
		const asio::ip::tcp::endpoint connEndPoint = mSocket.remote_endpoint();
		return std::hash<asio::ip::tcp::endpoint>{}(connEndPoint);
	}

	void HttpClientSessionAsio::Start() {}

	bool HttpClientSessionAsio::End()
	{
		if (mSocket.is_open()) {
			mSocket.close();
			return true;
		}
		return false;
	}

	bool HttpClientSessionAsio::IsAlive() const
	{
		return mSocket.is_open();
	}


	Time::TimePoint HttpClientSessionAsio::GetLastStartRequest() const
	{
		return mLastStartRequest;
	}

	void HttpClientSessionAsio::SendRequest(HttpRequest request, HttpResponseCallback callback)
	{
		auto req = std::make_shared<HttpRequest>(std::move(request));
		_tryConnectAndSendRequest(req, std::move(callback));
	}

	void HttpClientSessionAsio::SendEventRequest(const HttpUrl& url, HttpServerSentEventCallback callback)
	{
		auto request = std::make_shared<HttpRequest>(url, HttpRequest::Method::Get);
		request->GetHeader().SetConnection(HttpHeader::FieldConnection::KeepAlive);
		request->GetHeader().SetCacheControl(HttpHeader::FieldCacheControl::NoCache);
		request->GetHeader().SetContentType(HttpHeader::FieldContentType::EventStream);
		_startConnection(request->GetUrl().GetHost(), request->GetUrl().GetPort(), [this, request, fn = std::move(callback)](const std::error_code& ec) {
			_sendSSE(request, fn);
		});
	}

	void HttpClientSessionAsio::_startConnection(const std::string& host, size_t port, std::function<void(const std::error_code&)> callback)
	{
		mLastStartRequest = Time::Now();
		auto timer = AsioUtils::createTimeout(mClient->GetIO(), mClient->GetConfig().mConnTimeout, [callback](const std::error_code& e) {
			callback(e);
		});
		mResolver.async_resolve(host, std::to_string(port), [this, fn = callback, timer](const asio::error_code& ec, asio::ip::tcp::resolver::results_type endpoints) {
			if (ec) {
				fn(ec);
				return;
			}
			asio::async_connect(mSocket, endpoints, [this, fn, timer](const asio::error_code& ec, asio::ip::tcp::endpoint) {
				timer->cancel();
				fn(ec);
			});
		});
	}

	void HttpClientSessionAsio::_tryConnectAndSendRequest(std::shared_ptr<HttpRequest> request, HttpResponseCallback callback)
	{
		if (IsAlive()) {
			_sendRequest(request, std::move(callback));
		}
		else {
			_startConnection(request->GetUrl().GetHost(), request->GetUrl().GetPort(), [this, request, fn = std::move(callback)](const std::error_code& ec) {
				if (ec) {
					fn({}, ec);
					return;
				}
				_sendRequest(request, fn);
			});
		}
	}

	void HttpClientSessionAsio::_sendRequest(std::shared_ptr<HttpRequest> request, HttpResponseCallback callback)
	{
		auto requestStr = std::make_shared<std::string>(request->ToString());
		asio::async_write(mSocket, asio::buffer(*requestStr), [this, fn = std::move(callback), requestStr](const asio::error_code& ec, std::size_t bytesTransferred) {
			if (ec) {
				fn({}, ec);
				return;
			}
			_readResponse(fn);
		});
	}

	void HttpClientSessionAsio::_readResponse(HttpResponseCallback callback)
	{
		auto reader = std::make_shared<AsioUtils::Http::PayloadReader>(mSocket, std::make_shared<HttpResponse>());
		auto timer = AsioUtils::createTimeout(mClient->GetIO(), mClient->GetConfig().mResponseTimeout, [this, callback](const std::error_code& ec) {
			End();
		});
		AsioUtils::Http::readPayload(reader, [callback, timer](std::shared_ptr<HttpPayload> payload, std::error_code err) {
			timer->cancel();
			std::optional<HttpResponse> response;
			if (payload) {
				response = std::move(*std::static_pointer_cast<HttpResponse>(payload));
			}
			callback(response, err);
		});
	}

	void HttpClientSessionAsio::_sendSSE(std::shared_ptr<HttpRequest> request, HttpServerSentEventCallback callback)
	{
		auto requestStr = std::make_shared<std::string>(request->ToString());
		asio::async_write(mSocket, asio::buffer(*requestStr), [this, requestStr](const asio::error_code& ec, std::size_t bytesTransferred) {
			// TODO: Maybe handle close request here
		});
		_readSSE(std::move(callback));
	}

	void HttpClientSessionAsio::_readSSE(HttpServerSentEventCallback callback)
	{
		auto reader = std::make_shared<AsioUtils::Http::PayloadReader>(mSocket);
		AsioUtils::Http::readServerEventStream(reader, [fn = std::move(callback)](const HttpServerSentEvent* payload, std::error_code err) {
			fn(payload, err);
		});
	}
}