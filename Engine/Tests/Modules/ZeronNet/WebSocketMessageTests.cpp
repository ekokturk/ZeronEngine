// Copyright (C) Eser Kokturk. All Rights Reserved.

#include <Net/Http/HttpUrl.h>
#include <ZeronTestFramework.h>

#include <Net/WebSocket/WebSocketClient.h>
#include <Net/WebSocket/WebSocketServer.h>

using namespace ::Zeron;
using namespace ::Zeron::Net;

namespace ZeronNetTests
{
	class WebSocketMessageTests : public ::testing::Test {
	  public:
		static constexpr size_t SERVER_PORT = 11122;
		static inline const std::string SERVER_IP = Util::Format("127.0.0.1:{}", SERVER_PORT);

		void SetUp() override
		{
			mClient = WebSocketClient::Create();
			mServer = WebSocketServer::Create();
		}

		void TearDown() override {}

		void Update(std::function<bool()> condition, float timeout = 3.f)
		{
			Time::Timer<float, Time::Seconds> timer;
			while (!condition()) {
				if (timer.HasTimeElapsed(timeout)) {
					FAIL();
				}
				mServer->Update();
				mClient->Update();
			}
		}

	  protected:
		std::unique_ptr<WebSocketServer> mServer;
		std::unique_ptr<WebSocketClient> mClient;
	};

	TEST_F(WebSocketMessageTests, CreateClientServer_MessageClientOnConnection_SentAndRecievedMessages)
	{
		size_t connId = 0;
		bool handled = false;
		mClient->Connect(HttpUrl(SERVER_IP), [client = mClient.get(), &handled](const WebSocketMessage& msg) {
			switch (msg.mType) {
				case WebSocketMessage::Open: break;
				case WebSocketMessage::Close: break;
				case WebSocketMessage::Error: break;
				case WebSocketMessage::Text: {
					EXPECT_EQ("Server Responded", msg.mPayload);
					handled = true;
				} break;
				default: {
					FAIL();
				}
			}
		});
		mServer->Start(SERVER_PORT, [server = mServer.get(), &connId](const WebSocketMessage& msg) {
			switch (msg.mType) {
				case WebSocketMessage::Open: {
					connId = msg.mConnId;
					server->SendText("Server Responded", connId);
				} break;
				case WebSocketMessage::Close: break;
				case WebSocketMessage::Error: break;
				default: {
					FAIL();
				}
			}
		});

		Update([&handled]() {
			return handled;
		});
	}


	TEST_F(WebSocketMessageTests, CreateClientServer_SendShortText_SentAndRecievedMessages)
	{
		size_t connId = 0;
		bool handled = false;
		mClient->Connect(HttpUrl(SERVER_IP), [client = mClient.get(), &handled](const WebSocketMessage& msg) {
			switch (msg.mType) {
				case WebSocketMessage::Open: {
					client->SendText("Client Connected");
				} break;
				case WebSocketMessage::Close: break;
				case WebSocketMessage::Error: break;
				case WebSocketMessage::Text: {
					EXPECT_EQ("Server Responded", msg.mPayload);
					handled = true;
				} break;
				case WebSocketMessage::Binary: break;
				case WebSocketMessage::Ping: break;
				case WebSocketMessage::Pong: break;
			}
		});
		mServer->Start(SERVER_PORT, [server = mServer.get(), &connId](const WebSocketMessage& msg) {
			switch (msg.mType) {
				case WebSocketMessage::Open: {
					connId = msg.mConnId;
				} break;
				case WebSocketMessage::Close: break;
				case WebSocketMessage::Error: break;
				case WebSocketMessage::Text: {
					EXPECT_EQ("Client Connected", msg.mPayload);
					EXPECT_EQ(connId, msg.mConnId);
					server->SendText("Server Responded", connId);
				} break;
				case WebSocketMessage::Binary: break;
				case WebSocketMessage::Ping: break;
				case WebSocketMessage::Pong: break;
			}
		});

		Update([&handled]() {
			return handled;
		});
	}

	TEST_F(WebSocketMessageTests, CreateClientServer_SendLongText_SentAndRecievedMessages)
	{
		std::string payload(100000, 'A');
		size_t connId = 0;
		bool handled = false;
		mClient->Connect(HttpUrl(SERVER_IP), [client = mClient.get(), &handled, &payload](const WebSocketMessage& msg) {
			switch (msg.mType) {
				case WebSocketMessage::Open: {
					client->SendText(payload);
				} break;
				case WebSocketMessage::Close: break;
				case WebSocketMessage::Text: {
					EXPECT_EQ(payload, msg.mPayload);
					handled = true;
				} break;
				default: {
					FAIL();
				}
			}
		});
		mServer->Start(SERVER_PORT, [server = mServer.get(), &connId, &payload](const WebSocketMessage& msg) {
			switch (msg.mType) {
				case WebSocketMessage::Open: {
					connId = msg.mConnId;
				} break;
				case WebSocketMessage::Close: break;
				case WebSocketMessage::Text: {
					EXPECT_EQ(payload, msg.mPayload);
					EXPECT_EQ(connId, msg.mConnId);
					server->SendText(payload, connId);
				} break;
				default: {
					FAIL();
				}
			}
		});

		Update([&handled]() {
			return handled;
		});
	}

	TEST_F(WebSocketMessageTests, CreateClientServer_EndClientConnection_ConnectionsClosed)
	{
		size_t connId = 0;
		bool handled = false;
		mClient->Connect(HttpUrl(SERVER_IP), [client = mClient.get(), &handled](const WebSocketMessage& msg) {
			switch (msg.mType) {
				case WebSocketMessage::Open: {
					client->End();
				} break;
				case WebSocketMessage::Close: {
					handled = true;
				} break;
				case WebSocketMessage::Error: break;
				default: {
					FAIL();
				}
			}
		});
		mServer->Start(SERVER_PORT, [server = mServer.get(), &connId](const WebSocketMessage& msg) {
			switch (msg.mType) {
				case WebSocketMessage::Open: {
					connId = msg.mConnId;
				} break;
				case WebSocketMessage::Close: break;
				case WebSocketMessage::Error: break;
				default: {
					FAIL();
				}
			}
		});

		Update([&handled, client = mClient.get(), server = mServer.get(), &connId]() {
			return handled && WebSocketConnectionState::Idle == client->GetConnectionState() && !server->HasSession(connId);
		});
	}

	TEST_F(WebSocketMessageTests, CreateClientServer_EndServerConnection_ConnectionsClosed)
	{
		size_t connId = 0;
		bool clientHandled = false;
		bool serverHandled = false;
		mClient->Connect(HttpUrl(SERVER_IP), [&clientHandled](const WebSocketMessage& msg) {
			switch (msg.mType) {
				case WebSocketMessage::Open: break;
				case WebSocketMessage::Close: {
					clientHandled = true;
				} break;
				case WebSocketMessage::Error: break;
				default: {
					FAIL();
				}
			}
		});
		mServer->Start(SERVER_PORT, [server = mServer.get(), &connId, &serverHandled](const WebSocketMessage& msg) {
			switch (msg.mType) {
				case WebSocketMessage::Open: {
					connId = msg.mConnId;
					server->EndSession(msg.mConnId);
				} break;
				case WebSocketMessage::Close: {
					serverHandled = true;
				} break;
				case WebSocketMessage::Error: break;
				default: {
					FAIL();
				}
			}
		});

		Update([&clientHandled, &serverHandled, client = mClient.get(), server = mServer.get(), &connId]() {
			return clientHandled && serverHandled && WebSocketConnectionState::Idle == client->GetConnectionState() && !server->HasSession(connId);
		});
	}
}