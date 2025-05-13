// Copyright (C) Eser Kokturk. All Rights Reserved.

#include <Net/Http/HttpClient.h>

#include <Net/API/asio/HttpClientAsio.h>

namespace Zeron::Net
{

	std::unique_ptr<HttpClient> HttpClient::Create(const HttpClientConfig& config)
	{
		return std::make_unique<HttpClientAsio>(config);
	}
}