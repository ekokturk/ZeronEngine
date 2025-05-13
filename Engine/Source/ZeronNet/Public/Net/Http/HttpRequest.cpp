// Copyright (C) Eser Kokturk. All Rights Reserved.

#include <Net/Http/HttpRequest.h>


namespace Zeron::Net
{
	HttpRequest::HttpRequest(const std::string& url, Method method, std::optional<HttpHeader> header)
		: mUrl(url)
		, mMethod(method)
	{
		if (header.has_value()) {
			mHeader = std::move(header.value());
		}
	}

	HttpRequest::HttpRequest(const HttpUrl& url, Method method, std::optional<HttpHeader> header)
		: mUrl(url)
		, mMethod(method)
	{
		if (header.has_value()) {
			mHeader = std::move(header.value());
		}
	}


	std::string HttpRequest::GetHeaderString() const
	{
		std::string result = Util::Format("{} {} {}{}", GetMethodString(mMethod), mUrl.GetFullPath(), VERSION, "\r\n");

		result += Util::Format("Host: {}", mUrl.GetHost());
		if (mUrl.GetPort() != 80 && mUrl.GetPort() != 443) {
			result += Util::Format(":{}", mUrl.GetPort());
		}
		result += "\r\n";

		result += mHeader.ToString([](const std::string& key, const std::string&) {
			return true;
		});
		return result;
	}

	bool HttpRequest::ParseHeader(std::string_view rawStr)
	{
		const size_t requestLineEnd = rawStr.find("\r\n");
		if (requestLineEnd == std::string_view::npos) {
			return false;
		}

		const std::string_view requestLine = rawStr.substr(0, requestLineEnd);
		rawStr.remove_prefix(requestLineEnd + 2);

		std::istringstream requestStream{ std::string(requestLine) };
		std::string methodStr;
		std::string path;
		std::string version;

		requestStream >> methodStr >> path >> version;
		if (requestStream.fail() || version != VERSION) {
			return false;
		}

		if (methodStr == "GET") {
			mMethod = Method::Get;
		}
		else if (methodStr == "POST") {
			mMethod = Method::Post;
		}
		else if (methodStr == "PUT") {
			mMethod = Method::Put;
		}
		else if (methodStr == "DELETE") {
			mMethod = Method::Delete;
		}
		else if (methodStr == "PATCH") {
			mMethod = Method::Patch;
		}
		else {
			return false;
		}


		HttpHeader header{};
		header.FromString(rawStr);
		mHeader = std::move(header);

		mUrl = HttpUrl(path);


		return true;
	}

	const HttpUrl& HttpRequest::GetUrl() const
	{
		return mUrl;
	}

	HttpRequest::Method HttpRequest::GetMethod() const
	{
		return mMethod;
	}

	std::string HttpRequest::GetMethodString(Method method)
	{
		switch (method) {
			case Method::Get: return "GET";
			case Method::Post: return "POST";
			case Method::Put: return "PUT";
			case Method::Patch: return "PATCH";
			case Method::Delete: return "DELETE";
			default: {
				ZE_FAIL("HttpRequest: Invalid method");
				return "INVALID";
			};
		}
	}
}