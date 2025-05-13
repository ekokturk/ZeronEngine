// Copyright (C) Eser Kokturk. All Rights Reserved.

#include <Net/API/asio/HttpServerAsio.h>

#include <asio/connect.hpp>
#include <asio/ip/tcp.hpp>
#include <Net/API/asio/HttpServerSessionAsio.h>
#include <Net/Http/HttpResponse.h>

namespace Zeron::Net
{
	HttpServerAsio::HttpServerAsio()
		: mIO(std::make_unique<asio::io_context>())
		, mAcceptor(*mIO)
	{}

	HttpServerAsio::~HttpServerAsio()
	{
		mIO->stop();
	}

	bool HttpServerAsio::Start(size_t port, RequestCallback callback)
	{
		if (mAcceptor.is_open()) {
			return false;
		}

		mRequestCallback = std::move(callback);
		const asio::ip::tcp::endpoint endpoint(asio::ip::tcp::v4(), static_cast<unsigned short>(port));

		mAcceptor.open(endpoint.protocol());
		mAcceptor.set_option(asio::socket_base::reuse_address(true));
		mAcceptor.bind(endpoint);
		mAcceptor.listen();

		_acceptConnection();

		return true;
	}

	void HttpServerAsio::Stop()
	{
		if (mAcceptor.is_open()) {
			mRequestCallback = nullptr;
			mAcceptor.close();
		}
	}

	void HttpServerAsio::Update()
	{
		mIO->poll();
	}

	bool HttpServerAsio::HasSession(size_t connId) const
	{
		return mConnections.contains(connId);
	}

	void HttpServerAsio::SendEvent(size_t connId, const HttpServerSentEvent& evt, std::function<void(const std::error_code&)> callback)
	{
		auto itr = mConnections.find(connId);
		if (itr == mConnections.end()) {
			callback(std::make_error_code(std::errc::not_connected));
			return;
		}
		itr->second->SendEvent(evt, callback);
	}

	std::optional<HttpResponse> HttpServerAsio::OnRequest(size_t connId, std::shared_ptr<HttpRequest> request)
	{
		return mRequestCallback != nullptr ? mRequestCallback(connId, *request) : std::nullopt;
	}

	void HttpServerAsio::OnClose(size_t connId)
	{
		auto itr = mConnections.find(connId);
		if (itr != mConnections.end()) {
			mConnections.erase(connId);
		}
	}

	void HttpServerAsio::_acceptConnection()
	{
		auto socket = std::make_shared<asio::ip::tcp::socket>(*mIO);

		mAcceptor.async_accept(*socket, [this, socket](std::error_code ec) {
			if (!ec) {
				auto session = std::make_unique<HttpServerSessionAsio>(*this, std::move(*socket));
				const size_t id = session->GetId();
				mConnections[id] = std::move(session);
			}
			_acceptConnection();
		});
	}


}