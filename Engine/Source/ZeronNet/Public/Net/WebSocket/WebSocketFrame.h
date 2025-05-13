// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once


namespace Zeron::Net
{
	class WebSocketFrame {
	  public:
		enum class OpCode
		{
			Continuation = 0x0,
			Text = 0x1,
			Binary = 0x2,
			Close = 0x8,
			Ping = 0x9,
			Pong = 0xA,
		};

		WebSocketFrame() = default;
		WebSocketFrame(OpCode opCode, ByteBuffer payload = {}, bool isFinal = true, bool mask = true);

		ByteBuffer Serialize() const;
		std::optional<size_t> Deserialize(ByteBuffer::View data);
		bool operator==(const WebSocketFrame&) const = default;

		bool IsFinalFrame() const { return mFinal; }
		bool IsContinuationFrame() const { return mOpCode == OpCode::Continuation; }
		bool IsDataFrame() const { return mOpCode == OpCode::Text || mOpCode == OpCode::Binary; }
		bool HasMask() const { return mMask; }
		OpCode GetCode() const { return mOpCode; }
		const ByteBuffer& GetPayload() const { return mPayload; }

		static std::vector<WebSocketFrame> CreateFragments(const std::variant<ByteBuffer, std::string>& payload, size_t maxPayloadSize, bool mask = true);
		static Result<ByteBuffer, std::string> CreatePayloadFromFragments(const std::vector<WebSocketFrame>& fragments);


	  private:
		bool mFinal = true;
		bool mMask = false;
		OpCode mOpCode = OpCode::Close;
		ByteBuffer mPayload;
	};
}