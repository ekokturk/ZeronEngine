// Copyright (C) Eser Kokturk. All Rights Reserved.

#include <Net/WebSocket/WebSocketSession.h>

namespace Zeron::Net
{

	std::string WebSocketSession::GenerateKey()
	{
		constexpr int keyLength = 16;
		std::random_device rd;
		std::uniform_int_distribution<unsigned int> dist(0, 255);

		std::string key(keyLength, 0);
		std::transform(key.begin(), key.end(), key.begin(), [&dist, &rd](unsigned char) {
			return dist(rd);
		});

		std::string encodedKey = Util::EncodeBase64(key);

		return encodedKey;
	}

	std::string WebSocketSession::GenerateAcceptKey(const std::string& key)
	{
		constexpr char WEBSOCKET_GUID[] = "258EAFA5-E914-47DA-95CA-C5AB0DC85B11";
		const std::string acceptKey = key + WEBSOCKET_GUID;
		const std::string sha1 = Util::GenerateSHA1(acceptKey);
		return Util::EncodeBase64(sha1);
	}
}