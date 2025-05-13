// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once


namespace Zeron::Net
{
	class WebSocketMessage {
	  public:
		enum Type
		{
			Open,
			Close,
			Error,
			Text,
			Binary,
			Ping,
			Pong,
		};

		Type mType;
		std::string mPayload;
		size_t mConnId = 0;
	};

	using WebSocketMessageCallback = std::function<void(const WebSocketMessage& message)>;

	struct WebSocketMessageHandler {
		WebSocketMessageHandler(WebSocketMessageCallback callback)
			: mCallback(std::move(callback))
		{}
		~WebSocketMessageHandler() = default;

		void Handle(const WebSocketMessage& message) { mCallback(message); }

	  private:
		WebSocketMessageCallback mCallback;
	};
}