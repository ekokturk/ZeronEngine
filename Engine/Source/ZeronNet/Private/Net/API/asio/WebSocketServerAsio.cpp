// Copyright (C) Eser Kokturk. All Rights Reserved.

#include <Net/API/asio/WebSocketServerAsio.h>

#include <asio/connect.hpp>
#include <asio/ip/tcp.hpp>
#include <asio/write.hpp>
#include <Net/API/asio/AsioUtils.h>
#include <Net/API/asio/WebSocketSessionAsio.h>
#include <Net/Http/HttpRequest.h>
#include <Net/Http/HttpResponse.h>

namespace Zeron::Net
{
	WebSocketServerAsio::WebSocketServerAsio()
		: mIO(std::make_unique<asio::io_context>())
		, mAcceptor(*mIO)
	{}

	WebSocketServerAsio::~WebSocketServerAsio()
	{
		mIO->stop();
	}

	bool WebSocketServerAsio::Start(size_t port, WebSocketMessageCallback callback)
	{
		if (mAcceptor.is_open()) {
			return false;
		}

		const asio::ip::tcp::endpoint endpoint(asio::ip::tcp::v4(), static_cast<unsigned short>(port));
		asio::error_code ec;
		mAcceptor.open(endpoint.protocol(), ec);
		if (ec) {
			ZE_LOGE("WebSocketServer: Could not open the socket at port {}. {}", port, ec.message());
			mAcceptor.close();
			return false;
		}
		mAcceptor.bind(endpoint, ec);
		if (ec) {
			ZE_LOGE("WebSocketServer: Could not bind to port {}. {}", port, ec.message());
			mAcceptor.close();
			return false;
		}
		mAcceptor.listen(asio::socket_base::max_listen_connections, ec);
		if (ec) {
			ZE_LOGE("WebSocketServer: Could not listen on port {}. {}", port, ec.message());
			mAcceptor.close();
			return false;
		}

		mMessageHandler = std::make_unique<WebSocketMessageHandler>([this, fn = std::move(callback)](const WebSocketMessage& msg) {
			fn(msg);
			if (msg.mType == WebSocketMessage::Error) {
				EndSession(msg.mConnId);
			}
		});

		if (mIO->stopped()) {
			mIO->restart();
		}

		_acceptConnection();

		return true;
	}

	void WebSocketServerAsio::Stop()
	{
		if (mAcceptor.is_open()) {
			mConnections.clear();
			asio::error_code ec;
			mAcceptor.cancel();
			mAcceptor.close(ec);
			if (ec) {
				ZE_LOGE("WebSocketServer: Encountered an error while stopping '{}'", ec.message());
			}
			mIO->stop();
		}
	}

	void WebSocketServerAsio::Update()
	{
		mIO->poll();
	}

	bool WebSocketServerAsio::IsRunning() const
	{
		return mAcceptor.is_open();
	}

	std::vector<size_t> WebSocketServerAsio::GetSessionList() const
	{
		std::vector<size_t> list;
		list.reserve(mConnections.size());
		for (const auto& [key, _] : mConnections) {
			list.push_back(key);
		}
		return list;
	}

	bool WebSocketServerAsio::HasSession(size_t connId) const
	{
		return mConnections.contains(connId);
	}

	bool WebSocketServerAsio::EndSession(size_t connId)
	{
		if (WebSocketSession* session = _getSession(connId)) {
			session->End();
			mConnections.erase(connId);
			return true;
		}
		return false;
	}

	void WebSocketServerAsio::SendText(const std::string& message, size_t connId)
	{
		if (connId == 0) {
			for (auto&& conn : mConnections) {
				conn.second->SendText(message);
			}
		}
		else {
			if (WebSocketSession* session = _getSession(connId)) {
				session->SendText(message);
			}
		}
	}

	void WebSocketServerAsio::SendBinary(const ByteBuffer& message, size_t connId)
	{
		if (connId == 0) {
			for (auto&& conn : mConnections) {
				conn.second->SendBinary(message);
			}
		}
		else {
			if (WebSocketSession* session = _getSession(connId)) {
				session->SendBinary(message);
			}
		}
	}


	void WebSocketServerAsio::_acceptConnection()
	{
		auto socket = std::make_shared<asio::ip::tcp::socket>(*mIO);
		mAcceptor.async_accept(*socket, [this, socket](std::error_code ec) {
			if (!ec) {
				_handleHandshake(socket);
			}
			_acceptConnection();
		});
	}

	void WebSocketServerAsio::_handleHandshake(std::shared_ptr<asio::ip::tcp::socket> socket)
	{
		auto response = std::make_shared<AsioUtils::Http::PayloadReader>(*socket, std::make_shared<HttpRequest>());
		AsioUtils::Http::readPayload(response, [this, socket](std::shared_ptr<HttpPayload> payload, const std::error_code& err) {
			if (!err && payload) {
				std::shared_ptr<HttpRequest> request = std::static_pointer_cast<HttpRequest>(payload);
				const HttpHeader& header = request->GetHeader();
				const std::optional<std::string> key = request->GetHeader().Get("Sec-WebSocket-Key");
				if (header.GetConnection() == HttpHeader::FieldConnection::Upgrade && header.Get("Upgrade") == "websocket" && key.has_value()) {

					HttpResponse resp(HttpStatus::SwitchingProtocols);
					resp.GetHeader().Set("Sec-WebSocket-Accept", WebSocketSession::GenerateAcceptKey(*key));

					auto responseStr = std::make_shared<std::string>(resp.ToString());
					asio::async_write(*socket, asio::buffer(*responseStr), [this, responseStr, socket](const asio::error_code& ec, std::size_t bytesTransferred) {
						if (!ec) {
							auto session = std::make_unique<WebSocketSessionAsio>(std::move(*socket), mMessageHandler.get());
							const size_t id = session->GetId();
							mConnections[id] = std::move(session);
							mMessageHandler->Handle({ .mType = WebSocketMessage::Open, .mConnId = id });
						}
					});
				}
				else {
					mMessageHandler->Handle({ .mType = WebSocketMessage::Error, .mPayload = "Invalid WebSocket upgrade request" });
				}
			}
			else {
				mMessageHandler->Handle({ .mType = WebSocketMessage::Error, .mPayload = "Could not handle WebSocket upgrade request" });
			}
		});
	}

	WebSocketSession* WebSocketServerAsio::_getSession(size_t connId) const
	{
		if (auto conn = mConnections.find(connId); conn != mConnections.end()) {
			return conn->second.get();
		}
		return nullptr;
	}

}