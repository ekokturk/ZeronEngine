// Copyright (C) Eser Kokturk. All Rights Reserved.

#include <ZeronTestFramework.h>

#include <Net/Http/HttpClient.h>
#include <Net/Http/HttpHeader.h>
#include <Net/Http/HttpRequest.h>
#include <Net/Http/HttpResponse.h>
#include <Net/Http/HttpServer.h>

using namespace ::Zeron;
using namespace ::Zeron::Net;


namespace ZeronNetTests
{
	// TODO: Timeout
	// TODO: Multi Client

	class HttpMessageTests : public ::testing::Test {
	  public:
		static constexpr size_t SERVER_PORT = 11123;
		static inline const std::string SERVER_IP = Util::Format("127.0.0.1:{}", SERVER_PORT);

		void SetUp() override
		{
			mClient = HttpClient::Create();
			mServer = HttpServer::Create();
		}

		void TearDown() override {}

		void UpdateUntil(int reqCount, int resCount)
		{
			Time::Timer<float, Time::Seconds> timer;
			bool handled = false;
			while (true) {
				if (timer.HasTimeElapsed(3.f)) {
					break;
				}
				mServer->Update();
				mClient->Update();
				if (mFixture.mReqCount == reqCount && mFixture.mResCount == resCount) {
					handled = true;
					break;
				}
			}
			ASSERT_TRUE(handled);
		}

	  protected:
		struct Fixture {
			int mReqCount = 0;
			int mResCount = 0;
			bool mHandled = false;
		};

		Fixture mFixture;
		std::unique_ptr<HttpServer> mServer;
		std::unique_ptr<HttpClient> mClient;
	};


	TEST_F(HttpMessageTests, CreateClientServer_SendGetRequest_RequestAndResponseHandled)
	{
		size_t connId = 0;
		HttpRequest request{ SERVER_IP, HttpRequest::Get };
		request.SetBody("This should be discarded on server");

		mClient->SendRequest(std::move(request), [fixture = &mFixture](std::optional<HttpResponse> payload, const std::error_code& err) {
			++fixture->mResCount;
			ASSERT_TRUE(payload.has_value());
			ASSERT_TRUE(payload->GetHeader().GetContentType().has_value());
			EXPECT_EQ(HttpHeader::FieldContentType::PlainText, payload->GetHeader().GetContentType());
			EXPECT_EQ(HttpHeader::FieldConnection::Close, payload->GetHeader().GetConnection());
			EXPECT_EQ("Server Responded!", payload->GetBody());
			EXPECT_EQ(HttpStatus::OK, payload->GetStatus().mCode);
		});
		mServer->Start(SERVER_PORT, [fixture = &mFixture, &connId](size_t sessionId, const HttpRequest& request) {
			connId = sessionId;
			++fixture->mReqCount;
			EXPECT_EQ("/", request.GetUrl().GetPath());
			EXPECT_EQ(HttpRequest::Get, request.GetMethod());
			EXPECT_EQ("", request.GetBody());
			EXPECT_EQ(HttpHeader::FieldConnection::Close, request.GetHeader().GetConnection());
			auto response = HttpResponse(HttpStatus::OK);
			response.GetHeader().SetContentType(HttpHeader::FieldContentType::PlainText);
			response.GetHeader().SetConnection(HttpHeader::FieldConnection::Close);
			response.SetBody("Server Responded!");
			return response;
		});

		UpdateUntil(1, 1);
		EXPECT_FALSE(mServer->HasSession(connId));
	}

	TEST_F(HttpMessageTests, CreateClientServer_SendPostRequest_RequestAndResponseHandled)
	{
		size_t connId = 0;
		const std::string reqBody = "My paYload\n\n//stestS";
		const std::string resBody = "SerVer ResPonDed!";
		HttpHeader reqHeader;
		reqHeader.SetCacheControl(HttpHeader::FieldCacheControl::NoCache);
		HttpRequest request{
			SERVER_IP + "/data",
			HttpRequest::Post,
		};
		request.SetBody(reqBody);

		mClient->SendRequest(std::move(request), [fixture = &mFixture, &resBody](std::optional<HttpResponse> payload, const std::error_code& err) {
			++fixture->mResCount;
			ASSERT_TRUE(payload.has_value());
			ASSERT_TRUE(payload->GetHeader().GetContentType().has_value());
			EXPECT_EQ(HttpHeader::FieldContentType::Html, payload->GetHeader().GetContentType());
			EXPECT_EQ(HttpHeader::FieldConnection::KeepAlive, payload->GetHeader().GetConnection());
			EXPECT_EQ(resBody, payload->GetBody());
			EXPECT_EQ(resBody.size(), payload->GetHeader().GetContentLength());
			EXPECT_EQ(HttpStatus::OK, payload->GetStatus().mCode);
		});
		mServer->Start(SERVER_PORT, [fixture = &mFixture, &reqBody, &resBody, &connId](size_t sessionId, const HttpRequest& request) {
			++fixture->mReqCount;
			connId = sessionId;
			EXPECT_EQ("/data", request.GetUrl().GetPath());
			EXPECT_EQ(HttpRequest::Post, request.GetMethod());
			EXPECT_EQ(reqBody, request.GetBody());
			EXPECT_EQ(reqBody.size(), request.GetHeader().GetContentLength());
			auto response = HttpResponse(HttpStatus::OK);
			response.GetHeader().SetContentType(HttpHeader::FieldContentType::Html);
			response.GetHeader().SetConnection(HttpHeader::FieldConnection::KeepAlive);
			response.SetBody(resBody);
			return response;
		});

		UpdateUntil(1, 1);
		EXPECT_TRUE(mServer->HasSession(connId));
	}

	TEST_F(HttpMessageTests, CreateClientServer_SendDeleteRequest_RequestAndResponseHandled)
	{
		size_t connId = 0;
		const std::string reqBody = "My paYload\n\n//stestS";
		HttpHeader reqHeader;
		reqHeader.SetCacheControl(HttpHeader::FieldCacheControl::NoStore);
		HttpRequest request{
			SERVER_IP + "/data?id=1256",
			HttpRequest::Delete,
		};
		request.SetBody(reqBody);

		mClient->SendRequest(std::move(request), [fixture = &mFixture](std::optional<HttpResponse> payload, const std::error_code& err) {
			++fixture->mResCount;
			ASSERT_TRUE(payload.has_value());
			ASSERT_TRUE(payload->GetHeader().GetContentType().has_value());
			EXPECT_EQ(HttpHeader::FieldContentType::Html, payload->GetHeader().GetContentType());
			EXPECT_EQ(HttpHeader::FieldConnection::KeepAlive, payload->GetHeader().GetConnection());
			EXPECT_EQ("Server Responded!", payload->GetBody());
			EXPECT_EQ(HttpStatus::OK, payload->GetStatus().mCode);
		});
		mServer->Start(SERVER_PORT, [fixture = &mFixture, &connId, &reqBody](size_t sessionId, const HttpRequest& request) {
			++fixture->mReqCount;
			connId = sessionId;
			EXPECT_EQ("/data", request.GetUrl().GetPath());
			EXPECT_EQ("id=1256", request.GetUrl().GetQuery());
			EXPECT_EQ(HttpRequest::Delete, request.GetMethod());
			EXPECT_EQ(reqBody, request.GetBody());
			EXPECT_EQ(reqBody.size(), request.GetHeader().GetContentLength());
			auto response = HttpResponse(HttpStatus::OK);
			response.GetHeader().SetContentType(HttpHeader::FieldContentType::Html);
			response.GetHeader().SetConnection(HttpHeader::FieldConnection::KeepAlive);
			response.SetBody("Server Responded!");
			return response;
		});

		UpdateUntil(1, 1);
		EXPECT_TRUE(mServer->HasSession(connId));
	}

	TEST_F(HttpMessageTests, CreateClientServer_SendPutRequestAndRespondError_RequestAndResponseHandled)
	{
		size_t connId = 0;
		const std::string reqBody = "My paYload\n\n//stestS";
		const std::string resBody = "\n\r{server: error-data}\n\r";
		HttpHeader reqHeader;
		HttpRequest request{
			SERVER_IP + "/data?id=tesd&data=data#format",
			HttpRequest::Put,
		};
		request.SetBody(reqBody);

		mClient->SendRequest(std::move(request), [fixture = &mFixture, &resBody](std::optional<HttpResponse> payload, const std::error_code& err) {
			++fixture->mResCount;
			ASSERT_TRUE(payload.has_value());
			ASSERT_TRUE(payload->GetHeader().GetContentType().has_value());
			EXPECT_EQ(HttpHeader::FieldContentType::Json, payload->GetHeader().GetContentType());
			EXPECT_EQ(HttpHeader::FieldConnection::Close, payload->GetHeader().GetConnection());
			EXPECT_EQ(resBody, payload->GetBody());
			EXPECT_EQ(HttpStatus::NotImplemented, payload->GetStatus().mCode);
		});
		mServer->Start(SERVER_PORT, [fixture = &mFixture, &connId, &reqBody, &resBody](size_t sessionId, const HttpRequest& request) {
			++fixture->mReqCount;
			connId = sessionId;
			EXPECT_EQ("/data", request.GetUrl().GetPath());
			EXPECT_EQ("id=tesd&data=data", request.GetUrl().GetQuery());
			EXPECT_EQ("", request.GetUrl().GetFragment());
			EXPECT_EQ(HttpRequest::Put, request.GetMethod());
			EXPECT_EQ(reqBody, request.GetBody());
			EXPECT_EQ(reqBody.size(), request.GetHeader().GetContentLength());
			auto response = HttpResponse(HttpStatus::NotImplemented);
			response.GetHeader().SetContentType(HttpHeader::FieldContentType::Json);
			response.GetHeader().SetConnection(HttpHeader::FieldConnection::Close);
			response.SetBody(resBody);
			return response;
		});

		UpdateUntil(1, 1);
		EXPECT_FALSE(mServer->HasSession(connId));
	}


	TEST_F(HttpMessageTests, CreateClientServer_SendPatchRequestAndRespondError_RequestAndResponseHandled)
	{
		size_t connId = 0;
		const std::string reqBody = "My paYload\n\n//stestS";
		const std::string resBody = "\n\r{server: error-data}\n\r";
		HttpHeader reqHeader;
		HttpRequest request{
			SERVER_IP + "/data/info?id=test",
			HttpRequest::Patch,
		};
		request.SetBody(reqBody);

		mClient->SendRequest(std::move(request), [fixture = &mFixture, &resBody](std::optional<HttpResponse> payload, const std::error_code& err) {
			++fixture->mResCount;
			ASSERT_TRUE(payload.has_value());
			ASSERT_TRUE(payload->GetHeader().GetContentType().has_value());
			EXPECT_EQ(HttpHeader::FieldContentType::Xml, payload->GetHeader().GetContentType());
			EXPECT_EQ(HttpHeader::FieldConnection::Close, payload->GetHeader().GetConnection());
			EXPECT_EQ(resBody, payload->GetBody());
			EXPECT_EQ(HttpStatus::NotFound, payload->GetStatus().mCode);
		});
		mServer->Start(SERVER_PORT, [fixture = &mFixture, &connId, &reqBody, &resBody](size_t sessionId, const HttpRequest& request) {
			++fixture->mReqCount;
			connId = sessionId;
			EXPECT_EQ("/data/info", request.GetUrl().GetPath());
			EXPECT_EQ("id=test", request.GetUrl().GetQuery());
			EXPECT_EQ(HttpRequest::Patch, request.GetMethod());
			EXPECT_EQ(reqBody, request.GetBody());
			EXPECT_EQ(reqBody.size(), request.GetHeader().GetContentLength());
			auto response = HttpResponse(HttpStatus::NotFound);
			response.GetHeader().SetContentType(HttpHeader::FieldContentType::Xml);
			response.GetHeader().SetConnection(HttpHeader::FieldConnection::Close);
			response.SetBody(resBody);
			return response;
		});

		UpdateUntil(1, 1);
		EXPECT_FALSE(mServer->HasSession(connId));
	}

	TEST_F(HttpMessageTests, CreateClientServer_SendThreeServerEventRequest_HandledThreeEvents)
	{
		size_t connId = 0;
		HttpHeader reqHeader;
		const HttpUrl url{ SERVER_IP + "/data/events?id=test" };

		mClient->SendEventRequest(url, [fixture = &mFixture](const HttpServerSentEvent* payload, const std::error_code& err) {
			++fixture->mResCount;
			if (fixture->mResCount == 1) {
				EXPECT_EQ("first", payload->mId);
				EXPECT_EQ("myEvent", payload->mEvent);
				EXPECT_EQ("\n\n\ntest\n", payload->mData);
			}
			else if (fixture->mResCount == 2) {
				EXPECT_EQ("second", payload->mId);
				EXPECT_EQ("", payload->mEvent);
				EXPECT_EQ("\n\n\n\rtest", payload->mData);
			}
			else if (fixture->mResCount == 3) {
				EXPECT_EQ("", payload->mId);
				EXPECT_EQ("", payload->mEvent);
				EXPECT_EQ("test\n\n\n", payload->mData);
			}
		});
		mServer->Start(SERVER_PORT, [fixture = &mFixture, &connId, server = mServer.get()](size_t sessionId, const HttpRequest& request) {
			++fixture->mReqCount;
			connId = sessionId;
			EXPECT_EQ("/data/events", request.GetUrl().GetPath());
			EXPECT_EQ("id=test", request.GetUrl().GetQuery());
			EXPECT_EQ(HttpRequest::Get, request.GetMethod());
			EXPECT_EQ(HttpHeader::FieldContentType::EventStream, request.GetHeader().GetContentType());
			EXPECT_EQ(HttpHeader::FieldCacheControl::NoCache, request.GetHeader().GetCacheControl());
			server->SendEvent(sessionId, HttpServerSentEvent{ .mId = "first", .mEvent = "myEvent", .mData = "\n\n\ntest\n" });
			return std::nullopt;
		});

		UpdateUntil(1, 1);
		mServer->SendEvent(connId, HttpServerSentEvent{ .mId = "second", .mData = "\n\n\n\rtest" });
		UpdateUntil(1, 2);
		mServer->SendEvent(connId, HttpServerSentEvent{ .mData = "test\n\n\n" });
		UpdateUntil(1, 3);

		EXPECT_TRUE(mServer->HasSession(connId));
	}

	TEST_F(HttpMessageTests, CreateClientServer_SendRequestNotRespond_TimedOut)
	{
		size_t connId = 0;
		int messageCount = 0;
		auto client = HttpClient::Create({ .mResponseTimeout = Time::Milliseconds{ 10 } });
		auto server = HttpServer::Create();
		HttpRequest request{ SERVER_IP, HttpRequest::Get };
		request.SetBody("This should be discarded on server");

		client->SendRequest(std::move(request), [&messageCount](std::optional<HttpResponse> payload, const std::error_code& err) {
			++messageCount;
			EXPECT_FALSE(payload.has_value());
			EXPECT_TRUE(err);
		});

		server->Start(SERVER_PORT, [&connId](size_t sessionId, const HttpRequest& request) {
			connId = sessionId;
			return std::nullopt;
		});

		auto initTime = Time::Now();
		while (Time::CalculateElapsedTime<float, Time::Milliseconds>(Time::Now(), initTime) < 12) {
			client->Update();
			server->Update();
		}

		EXPECT_EQ(1, messageCount);
		EXPECT_FALSE(server->HasSession(connId));
	}

	TEST_F(HttpMessageTests, CreateOnlyClient_SendRequest_TimedOut)
	{
		int messageCount = 0;
		auto client = HttpClient::Create({ .mConnTimeout = Time::Milliseconds{ 10 } });
		HttpRequest request{ SERVER_IP, HttpRequest::Get };
		request.SetBody("This should be discarded on server");

		client->SendRequest(std::move(request), [&messageCount](std::optional<HttpResponse> payload, const std::error_code& err) {
			++messageCount;
			EXPECT_FALSE(payload.has_value());
			ASSERT_TRUE(err);
		});

		Time::Timer<float, Time::Milliseconds> timer;
		while (!timer.HasTimeElapsed(100.f)) {
			client->Update();
		}

		EXPECT_EQ(1, messageCount);
	}
}