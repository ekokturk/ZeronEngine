// Copyright (C) Eser Kokturk. All Rights Reserved.

#include <Net/Http/HttpServer.h>

#include <Net/API/asio/HttpServerAsio.h>

namespace Zeron::Net
{

	std::unique_ptr<HttpServer> HttpServer::Create()
	{
		return std::make_unique<HttpServerAsio>();
	}
}