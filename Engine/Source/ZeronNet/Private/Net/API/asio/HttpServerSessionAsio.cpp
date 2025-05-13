// Copyright (C) Eser Kokturk. All Rights Reserved.

#include <Net/API/asio/HttpServerSessionAsio.h>

#include <asio/connect.hpp>
#include <asio/ip/tcp.hpp>
#include <asio/write.hpp>
#include <Net/API/asio/AsioUtils.h>
#include <Net/API/asio/HttpServerAsio.h>
#include <Net/Http/HttpRequest.h>
#include <Net/Http/HttpResponse.h>
#include <Net/Http/HttpServerSentEvent.h>

namespace Zeron::Net
{
	HttpServerSessionAsio::HttpServerSessionAsio(HttpServerAsio& server, asio::ip::tcp::socket socket)
		: mSocket(std::move(socket))
		, mServer(&server)
		, mSessionType(HttpSessionType::Unknown)
	{
		_readRequest();
	}

	HttpServerSessionAsio::~HttpServerSessionAsio()
	{
		mSocket.close();
	}

	size_t HttpServerSessionAsio::GetId() const
	{
		const asio::ip::tcp::endpoint connEndPoint = mSocket.remote_endpoint();
		return std::hash<asio::ip::tcp::endpoint>{}(connEndPoint);
	}

	bool HttpServerSessionAsio::End()
	{
		if (mSocket.is_open()) {
			mSocket.close();
			return true;
		}
		return false;
	}

	void HttpServerSessionAsio::SendEvent(const HttpServerSentEvent& evt, std::function<void(const std::error_code&)> callback)
	{
		if (mSessionType == HttpSessionType::SSE) {
			auto dataPayload = std::make_shared<std::string>(evt.ToString());
			asio::async_write(mSocket, asio::buffer(*dataPayload), [this, dataPayload, fn = std::move(callback)](std::error_code ec, std::size_t) {
				if (ec) {
					fn(ec);
					mSocket.close();
				}
			});
		}
		else {
			// TODO: Error/Asser there
		}
	}

	void HttpServerSessionAsio::_readRequest()
	{
		auto response = std::make_shared<AsioUtils::Http::PayloadReader>(mSocket, std::make_shared<HttpRequest>());
		AsioUtils::Http::readPayload(response, [this, server = mServer](std::shared_ptr<HttpPayload> payload, const std::error_code& err) {
			if (!err) {
				const asio::ip::tcp::endpoint endPoint = mSocket.remote_endpoint();
				std::shared_ptr<HttpRequest> request = std::static_pointer_cast<HttpRequest>(payload);
				std::optional<HttpHeader::FieldContentType> contentType = request->GetHeader().GetContentType();
				// We discard message body if it exists for Get requsts
				if (request->GetMethod() == HttpRequest::Get) {
					request->SetBody("");
				}

				// SSE
				if (contentType.has_value() && *contentType == HttpHeader::FieldContentType::EventStream) {
					mSessionType = HttpSessionType::SSE;
					// We ignore the response for server sent event registration
					server->OnRequest(GetId(), request);
				}
				// REST
				else {
					mSessionType = HttpSessionType::Rest;
					if (std::optional<HttpResponse> response = server->OnRequest(GetId(), request)) {
						const HttpHeader::FieldConnection respConn = response->GetHeader().GetConnection();

						// Set generic headers
						response->GetHeader().SetDate(std::chrono::system_clock::now());

						// Send response then close the connection if requested
						_sendResponse(std::make_shared<HttpResponse>(std::move(*response)), respConn == HttpHeader::FieldConnection::Close);
					}
					else {
						// We close the connection if we won't respond back
						mServer->OnClose(GetId());
					}
				}
			}
		});
	}

	void HttpServerSessionAsio::_sendResponse(std::shared_ptr<HttpResponse> response, bool closeSocket)
	{
		auto responseStr = std::make_shared<std::string>(response->ToString());
		asio::async_write(mSocket, asio::buffer(*responseStr), [this, responseStr, server = mServer, closeSocket](const asio::error_code& ec, std::size_t bytesTransferred) {
			if (closeSocket) {
				mServer->OnClose(GetId());
			}
			else {
				if (!ec) {
					_readRequest();
				}
				else {
					ZE_LOG("HttpServerSessionAsio: Unable to send respose. {}", ec.message().c_str());
				}
			}
		});
	}


}