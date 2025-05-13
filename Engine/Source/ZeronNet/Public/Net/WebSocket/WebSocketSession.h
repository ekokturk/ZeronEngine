// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

namespace Zeron::Net
{
	class WebSocketSession {
	  public:
		virtual ~WebSocketSession() = default;

		static std::string GenerateKey();
		static std::string GenerateAcceptKey(const std::string& key);


		virtual void SendText(const std::string& message) = 0;
		virtual void SendBinary(const ByteBuffer& message) = 0;
		virtual size_t GetId() const = 0;
		virtual void End() = 0;
	};
}