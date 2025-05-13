// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

#include <Sandbox/Samples/IWindowSample.h>

namespace Zeron
{
	namespace Net
	{
		class WebSocketClient;
		class WebSocketServer;
	}
	class Window;
}

namespace Sandbox
{
	struct SampleContext;

	class WebSocketSample : public IWindowSample {
	  public:
		static constexpr char NAME[] = "WebSocket";

		WebSocketSample(IWindowSampleContext& ctx);
		~WebSocketSample() override;

		void Init() override;
		void Update() override;
		void OnGui() override;

	  private:
		struct Server {
			std::unique_ptr<Zeron::Net::WebSocketServer> mSession;
			std::vector<std::string> mMessages;
			char mStagedMessage[256] = {};
		};

		struct Client {
			std::unique_ptr<Zeron::Net::WebSocketClient> mSession;
			std::vector<std::string> mMessages;
			char mStagedMessage[256] = {};
		};

		std::unordered_map<size_t, Server> mServers;
		std::unordered_map<size_t, Client> mClients;
	};
}