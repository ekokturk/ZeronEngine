// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

#include <Net/Http/HttpPayload.h>
#include <Net/Http/HttpUrl.h>

namespace Zeron::Net
{
	class HttpRequest : public HttpPayload {
	  public:
		enum Method
		{
			Get = 0,
			Post,
			Put,
			Delete,
			Patch
		};

		HttpRequest() = default;
		HttpRequest(const std::string& url, Method method, std::optional<HttpHeader> header = {});
		HttpRequest(const HttpUrl& url, Method method, std::optional<HttpHeader> header = {});

		std::string GetHeaderString() const override;
		bool ParseHeader(std::string_view rawStr) override;

		const HttpUrl& GetUrl() const;
		Method GetMethod() const;
		static std::string GetMethodString(Method method);

	  private:
		HttpUrl mUrl;
		Method mMethod = Method::Get;
	};

	using HttpRequestError = std::error_code;
	using HttpRequestCallback = std::function<void(std::optional<HttpRequest>, const HttpRequestError&)>;
}