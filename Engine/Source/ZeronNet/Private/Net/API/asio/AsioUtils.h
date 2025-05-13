// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

#include <asio/ip/tcp.hpp>
#include <asio/steady_timer.hpp>
#include <Net/Http/HttpPayload.h>
#include <Net/Http/HttpServerSentEvent.h>


namespace Zeron::Net::AsioUtils
{
	namespace Http
	{
		struct PayloadReader {
			std::reference_wrapper<asio::ip::tcp::socket> mSocket;
			std::shared_ptr<HttpPayload> mPayload;
			std::string mData;
		};

		void readPayload(std::shared_ptr<PayloadReader> reader, std::function<void(std::shared_ptr<HttpPayload>, const std::error_code&)> callback);
		void readServerEventStream(std::shared_ptr<PayloadReader> reader, HttpServerSentEventCallback callback);
	}

	std::shared_ptr<asio::steady_timer> createTimeout(asio::io_context& ctx, Time::Milliseconds ms, std::function<void(const std::error_code&)> onTimeout);
}