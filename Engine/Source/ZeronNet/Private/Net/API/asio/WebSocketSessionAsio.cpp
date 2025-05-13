// Copyright (C) Eser Kokturk. All Rights Reserved.

#include <Net/API/asio/WebSocketSessionAsio.h>

#include <asio/ip/tcp.hpp>
#include <asio/write.hpp>
#include <Net/WebSocket/WebSocketFrame.h>
#include <Net/WebSocket/WebSocketMessage.h>

namespace Zeron::Net
{

	WebSocketSessionAsio::WebSocketSessionAsio(asio::ip::tcp::socket socket, WebSocketMessageHandler* handler, ByteBuffer rawData)
		: mSocket(std::move(socket))
		, mId(0)
		, mRawMessageBuffer(rawData)
		, mMessageHandler(handler)
	{
		const asio::ip::tcp::endpoint connEndPoint = mSocket.remote_endpoint();
		mId = std::hash<asio::ip::tcp::endpoint>{}(connEndPoint);
		_processRawMessageBuffer();
		_readMessage();
	}

	WebSocketSessionAsio::~WebSocketSessionAsio()
	{
		_closeConnection();
	}

	size_t WebSocketSessionAsio::GetId() const
	{
		return mId;
	}

	void WebSocketSessionAsio::SendText(const std::string& message)
	{
		_sendMessage(message);
	}

	void WebSocketSessionAsio::SendBinary(const ByteBuffer& message)
	{
		_sendMessage(message);
	}

	void WebSocketSessionAsio::End()
	{
		_closeConnection();
	}

	void WebSocketSessionAsio::_sendMessage(const std::variant<ByteBuffer, std::string>& payload)
	{
		const std::vector<WebSocketFrame> fragments = WebSocketFrame::CreateFragments(payload, 8092);
		for (const auto& fragment : fragments) {
			mOutgoingFragmentBuffer.push(fragment.Serialize());
		}
		if (!mIsProcessingOutgoingFragments && !mOutgoingFragmentBuffer.empty()) {
			_sendNextFragment();
			mIsProcessingOutgoingFragments = true;
		}
	}

	void WebSocketSessionAsio::_readMessage()
	{
		mSocket.async_read_some(asio::buffer(mReadBuffer), [this, handler = mMessageHandler, id = GetId()](std::error_code ec, size_t length) {
			if (ec) {
				if (handler) {
					handler->Handle({ .mType = WebSocketMessage::Error, .mPayload = "Could not read WebSocket message frame", .mConnId = id });
				}
				return;
			}

			mRawMessageBuffer.insert(mRawMessageBuffer.end(), reinterpret_cast<std::byte*>(mReadBuffer.data()), reinterpret_cast<std::byte*>(mReadBuffer.data()) + length);
			_processRawMessageBuffer();
			_readMessage();
		});
	}

	void WebSocketSessionAsio::_sendNextFragment()
	{
		asio::async_write(
			mSocket,
			asio::buffer(mOutgoingFragmentBuffer.front()),
			[this, handler = mMessageHandler, id = GetId()](const asio::error_code& ec, std::size_t bytesTransferred) {
				if (ec) {
					if (handler) {
						handler->Handle({ .mType = WebSocketMessage::Error, .mPayload = "Could not send WebSocket message frame", .mConnId = id });
					}
					return;
				}
				mOutgoingFragmentBuffer.pop();
				if (mOutgoingFragmentBuffer.empty()) {
					mIsProcessingOutgoingFragments = false;
				}
				else {
					_sendNextFragment();
				}
			}
		);
	}

	void WebSocketSessionAsio::_closeConnection()
	{
		if (mSocket.is_open()) {
			mMessageHandler->Handle({ .mType = WebSocketMessage::Close, .mConnId = GetId() });
			mSocket.close();
		}
	}

	void WebSocketSessionAsio::_processRawMessageBuffer()
	{
		size_t readOffset = 0;
		while (readOffset < mRawMessageBuffer.size()) {
			WebSocketFrame frame;
			const std::optional<size_t> readBytes = frame.Deserialize(mRawMessageBuffer.GetView(readOffset));
			if (!readBytes.has_value()) {
				break;
			}
			readOffset += *readBytes;

			const bool finalFragment = frame.IsFinalFrame();
			mIncomingFragmentBuffer.push_back(std::move(frame));

			if (finalFragment) {
				auto result = WebSocketFrame::CreatePayloadFromFragments(mIncomingFragmentBuffer);
				if (result.HasError()) {
					mMessageHandler->Handle({ .mType = WebSocketMessage::Type::Error, .mPayload = result.GetError(), .mConnId = GetId() });
				}
				else {
					const WebSocketMessage::Type type = mIncomingFragmentBuffer.front().GetCode() == WebSocketFrame::OpCode::Text ? WebSocketMessage::Text :
																																	WebSocketMessage::Binary;
					mMessageHandler->Handle(
						{ .mType = type, .mPayload = std::string(reinterpret_cast<const char*>(result.Value().data()), result.Value().size()), .mConnId = GetId() }
					);
				}
				mIncomingFragmentBuffer.clear();
			}
		}

		mRawMessageBuffer.erase(mRawMessageBuffer.begin(), mRawMessageBuffer.begin() + readOffset);
	}


}