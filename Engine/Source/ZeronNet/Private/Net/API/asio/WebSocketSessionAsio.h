// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

#include <asio/ip/tcp.hpp>
#include <Net/WebSocket/WebSocketFrame.h>
#include <Net/WebSocket/WebSocketMessage.h>
#include <Net/WebSocket/WebSocketSession.h>


namespace Zeron::Net
{
	class HttpClientSessionAsio;

	class WebSocketSessionAsio : public WebSocketSession {
	  public:
		WebSocketSessionAsio(asio::ip::tcp::socket socket, WebSocketMessageHandler* callback, ByteBuffer rawData = {});
		~WebSocketSessionAsio() override;

		size_t GetId() const override;
		void SendText(const std::string& message) override;
		void SendBinary(const ByteBuffer& message) override;
		void End() override;

	  private:
		void _sendMessage(const std::variant<ByteBuffer, std::string>& payload);
		void _readMessage();
		void _sendNextFragment();
		void _closeConnection();
		void _processRawMessageBuffer();

		asio::ip::tcp::socket mSocket;
		size_t mId;
		ByteBuffer mRawMessageBuffer;
		std::array<uint8_t, 2048> mReadBuffer{};
		WebSocketMessageHandler* mMessageHandler;

		std::vector<WebSocketFrame> mIncomingFragmentBuffer;

		std::queue<ByteBuffer> mOutgoingFragmentBuffer;
		bool mIsProcessingOutgoingFragments = false;
	};
}