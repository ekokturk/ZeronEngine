// Copyright (C) Eser Kokturk. All Rights Reserved.

#include <Net/WebSocket/WebSocketFrame.h>


namespace Zeron::Net
{

	WebSocketFrame::WebSocketFrame(OpCode opCode, ByteBuffer payload, bool isFinal, bool mask)
		: mFinal(isFinal)
		, mMask(mask)
		, mOpCode(opCode)
		, mPayload(std::move(payload))
	{}

	ByteBuffer WebSocketFrame::Serialize() const
	{
		ByteBuffer buff;

		// FIN + OpCode
		buff.push_back(static_cast<std::byte>((mFinal ? 0b10000000 : 0x00) | (static_cast<uint8_t>(mOpCode) & 0b00001111)));

		// Mask Flag + Payload Length
		uint8_t byte2 = mMask ? 0b10000000 : 0;
		const uint64_t payloadLength = mPayload.size();
		if (payloadLength <= 125) {
			byte2 |= static_cast<uint8_t>(payloadLength);
			buff.push_back(static_cast<std::byte>(byte2));
		}
		else if (payloadLength <= 65535) {	// 2^16 - 1
			byte2 |= 126;
			buff.push_back(static_cast<std::byte>(byte2));
			buff.push_back(static_cast<std::byte>((payloadLength >> 8) & 255));
			buff.push_back(static_cast<std::byte>(payloadLength & 255));
		}
		else {
			byte2 |= 127;
			buff.push_back(static_cast<std::byte>(byte2));
			for (int i = 7; i >= 0; --i) {
				buff.push_back(static_cast<std::byte>((payloadLength >> (i * 8)) & 255));
			}
		}

		if (mMask) {  // Mask key + Payload
			Random rand;
			std::array maskKey = { rand.GetByte(), rand.GetByte(), rand.GetByte(), rand.GetByte() };
			buff.insert(buff.end(), maskKey.begin(), maskKey.end());

			for (size_t i = 0; i < payloadLength; ++i) {
				auto masked = static_cast<std::byte>(std::to_integer<uint8_t>(mPayload[i]) ^ std::to_integer<uint8_t>(maskKey[i % 4]));
				buff.push_back(masked);
			}
		}
		else {	// Payload
			buff.insert(buff.end(), mPayload.begin(), mPayload.end());
		}

		return buff;
	}

	std::optional<size_t> WebSocketFrame::Deserialize(ByteBuffer::View data)
	{
		// We need to have at least 2 header bytes
		if (data.size() < 2) {
			return std::nullopt;
		}

		size_t pos = 0;
		// FIN + OpCode
		const uint8_t byte1 = static_cast<uint8_t>(data[pos++]);
		mFinal = byte1 & 0b10000000;
		mOpCode = static_cast<OpCode>(byte1 & 0b00001111);

		// Mask Flag + Payload Length
		const uint8_t byte2 = static_cast<uint8_t>(data[pos++]);
		mMask = byte2 & 0b10000000;
		uint64_t payloadLength = byte2 & 0b01111111;
		if (payloadLength == 126) {	 // Next 2 bytes are payload length
			if (data.size() < pos + 2) {
				return std::nullopt;
			}
			payloadLength = (std::to_integer<uint8_t>(data[pos]) << 8) | (std::to_integer<uint8_t>(data[pos + 1]));
			pos += 2;
		}
		else if (payloadLength == 127) {  // Next 8 bytes are payload length
			if (data.size() < pos + 8) {
				return std::nullopt;
			}
			payloadLength = 0;
			for (int i = 0; i < 8; ++i) {
				payloadLength = (payloadLength << 8) | std::to_integer<uint8_t>(data[pos++]);
			}
		}

		// Mask Key
		std::array<std::byte, 4> maskKey{};
		if (mMask) {
			if (data.size() < pos + 4) {
				return std::nullopt;
			}
			std::copy(data.begin() + pos, data.begin() + pos + 4, maskKey.begin());
			pos += 4;
		}

		if (data.size() < pos + payloadLength) {
			return std::nullopt;
		}

		// Payload
		mPayload.resize(payloadLength);
		std::copy(data.begin() + pos, data.begin() + pos + payloadLength, mPayload.begin());

		if (mMask) {
			for (size_t i = 0; i < payloadLength; ++i) {
				mPayload[i] = static_cast<std::byte>(std::to_integer<uint8_t>(mPayload[i]) ^ std::to_integer<uint8_t>(maskKey[i % 4]));
			}
		}
		pos += payloadLength;

		return pos;
	}

	std::vector<WebSocketFrame> WebSocketFrame::CreateFragments(const std::variant<ByteBuffer, std::string>& payload, size_t maxPayloadSize, bool mask)
	{
		std::vector<WebSocketFrame> frags;
		const ByteBuffer::View view = std::visit(
			TypeTraits::Visitor{
				[](const ByteBuffer& p) -> ByteBuffer::View {
					return p.GetView();
				},
				[](const std::string& p) -> ByteBuffer::View {
					return { reinterpret_cast<const std::byte*>(p.data()), p.size() };
				},
			},
			payload
		);

		size_t offset = 0;
		OpCode code = std::holds_alternative<std::string>(payload) ? OpCode::Text : OpCode::Binary;

		while (offset < view.size()) {
			const size_t chunkSize = std::min(maxPayloadSize, view.size() - offset);
			const bool isFinal = (offset + chunkSize) == view.size();
			ByteBuffer chunkPayload(view.begin() + offset, view.begin() + offset + chunkSize);

			WebSocketFrame frame(code, std::move(chunkPayload), isFinal, mask);
			frags.push_back(std::move(frame));

			offset += chunkSize;
			code = OpCode::Continuation;
		}

		return frags;
	}

	Result<ByteBuffer, std::string> WebSocketFrame::CreatePayloadFromFragments(const std::vector<WebSocketFrame>& fragments)
	{
		ByteBuffer buff;
		if (fragments.empty()) {
			return { buff };
		}

		const auto& firstFrag = fragments.front();
		if (firstFrag.IsFinalFrame()) {
			if (fragments.size() > 1) {
				return ErrorResult<std::string>{ "Expected only a single fragment to exist for the WebSocket frame" };
			}
			return { firstFrag.GetPayload() };
		}

		if (!firstFrag.IsDataFrame()) {
			return ErrorResult<std::string>{ "Expected fragmented WebSocket frame to be either text or binary payload" };
		}

		for (size_t i = 0; i < fragments.size(); ++i) {
			const auto& fragment = fragments[i];
			const ByteBuffer& payload = fragment.GetPayload();
			if (fragment.IsFinalFrame()) {
				if (i != (fragments.size() - 1)) {
					return ErrorResult<std::string>{ "Expected final WebSocket frame fragment to be the last" };
				}
				buff.Append(payload.GetView());
			}
			else if (fragment.IsContinuationFrame() || firstFrag.IsDataFrame()) {
				buff.Append(payload.GetView());
			}
			else {
				return ErrorResult<std::string>{ "Invalid operation code was found in the WebSocket frame fragment" };
			}
		}

		return { buff };
	}
}