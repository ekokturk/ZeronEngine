// Copyright (C) Eser Kokturk. All Rights Reserved.

#include <Net/API/asio/WebSocketClientAsio.h>

#include <asio/connect.hpp>
#include <asio/ip/tcp.hpp>
#include <asio/write.hpp>
#include <Net/API/asio/AsioUtils.h>
#include <Net/API/asio/WebSocketSessionAsio.h>
#include <Net/Http/HttpResponse.h>
#include <Net/Http/HttpUrl.h>

namespace Zeron::Net
{
	WebSocketClientAsio::WebSocketClientAsio()
		: mIO(std::make_unique<asio::io_context>())
		, mResolver(*mIO)
	{}

	WebSocketClientAsio::~WebSocketClientAsio()
	{
		mIO->stop();
	}

	void WebSocketClientAsio::Update()
	{
		mIO->poll();
	}

	void WebSocketClientAsio::Connect(const HttpUrl& url, WebSocketMessageCallback callback)
	{
		if (mSession || mConnectionState) {
			return;
		}

		mMessageHandler = std::make_unique<WebSocketMessageHandler>([this, fn = std::move(callback)](auto&& msg) {
			fn(msg);
			if (msg.mType == WebSocketMessage::Error) {
				End();
			}
		});

		if (mIO->stopped()) {
			mIO->restart();
		}

		mResolver.async_resolve(url.GetHost(), std::to_string(url.GetPort()), [this, url](const asio::error_code& ec, asio::ip::tcp::resolver::results_type endpoints) {
			if (ec) {
				mMessageHandler->Handle({ .mType = WebSocketMessage::Error, .mPayload = "Could not resolve WebSocket server" });
				return;
			}

			mConnectionState = std::make_unique<ConnectionState>(asio::ip::tcp::socket(*mIO), WebSocketSession::GenerateKey());
			asio::async_connect(mConnectionState->mSocket, endpoints, [this, url = std::move(url)](const asio::error_code& ec, asio::ip::tcp::endpoint) {
				if (ec) {
					mMessageHandler->Handle({ .mType = WebSocketMessage::Error, .mPayload = "Could not connect to WebSocket server" });
					return;
				}

				HttpHeader header;
				header.SetConnection(HttpHeader::FieldConnection::Upgrade);
				header.Set("Upgrade", "websocket");
				header.Set("Sec-WebSocket-Key", mConnectionState->mKey);
				header.Set("Sec-WebSocket-Version", "13");

				_handleHandshake(HttpRequest(url, HttpRequest::Method::Get, std::move(header)));
			});
		});
	}

	void WebSocketClientAsio::SendText(const std::string& message)
	{
		if (mSession) {
			mSession->SendText(message);
		}
	}

	void WebSocketClientAsio::SendBinary(const ByteBuffer& message)
	{
		if (mSession) {
			mSession->SendBinary(message);
		}
	}

	void WebSocketClientAsio::End()
	{
		if (mConnectionState) {
			mConnectionState->mSocket.close();
		}
		if (mSession) {
			mSession->End();
			mSession = nullptr;
		}
	}

	size_t WebSocketClientAsio::GetId() const
	{
		return mSession ? mSession->GetId() : 0;
	}

	WebSocketConnectionState WebSocketClientAsio::GetConnectionState() const
	{
		if (mConnectionState) {
			return WebSocketConnectionState::Connecting;
		}
		if (mSession) {
			return WebSocketConnectionState::Connected;
		}
		return WebSocketConnectionState::Idle;
	}

	asio::io_context& WebSocketClientAsio::GetIO()
	{
		return *mIO;
	}

	void WebSocketClientAsio::_handleHandshake(const HttpRequest& request)
	{
		auto requestStr = std::make_shared<std::string>(request.ToString());
		asio::async_write(mConnectionState->mSocket, asio::buffer(*requestStr), [this, requestStr](const asio::error_code& ec, std::size_t bytesTransferred) {
			if (ec) {
				mMessageHandler->Handle({ .mType = WebSocketMessage::Error, .mPayload = "Could not request WebSocket handshake" });
				return;
			}
			auto reader = std::make_shared<AsioUtils::Http::PayloadReader>(mConnectionState->mSocket, std::make_shared<HttpResponse>());
			AsioUtils::Http::readPayload(reader, [this, reader](std::shared_ptr<HttpPayload> payload, std::error_code ec) {
				if (payload && !ec) {
					const std::shared_ptr<HttpResponse> resp = std::static_pointer_cast<HttpResponse>(payload);
					// Need to check expected the status code and the server accept key to validate the connection
					if (resp->GetStatus().mCode != HttpStatus::SwitchingProtocols) {
						mMessageHandler->Handle({
							.mType = WebSocketMessage::Error,
							.mPayload = Util::Format("Invalid WebSocket handshake response: {}", resp->GetStatus().GetReason()),
						});
						return;
					}
					const std::optional<std::string> acceptKey = resp->GetHeader().Get("Sec-WebSocket-Accept");
					const std::string expectedKey = WebSocketSession::GenerateAcceptKey(mConnectionState->mKey);
					if (acceptKey.has_value() && acceptKey != expectedKey) {
						mMessageHandler->Handle({ .mType = WebSocketMessage::Error, .mPayload = "Invalid WebSocket handshake accept key" });
						return;
					}

					// Create the session and pass already read data to be processed as websocket frame
					mSession = std::make_unique<WebSocketSessionAsio>(
						std::move(mConnectionState->mSocket), mMessageHandler.get(), ByteBuffer::Create(std::span{ reader->mData })
					);
					mConnectionState = nullptr;
					mMessageHandler->Handle({ .mType = WebSocketMessage::Type::Open, .mConnId = mSession->GetId() });
				}
				else {
					ZE_FAIL("WebSocketClientAsio: Invalid handshake response");
				}
			});
		});
	}
}