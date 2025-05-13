// Copyright (C) Eser Kokturk. All Rights Reserved.

#include <Net/Http/HttpResponse.h>

namespace Zeron::Net
{
	HttpResponse::HttpResponse() {}

	HttpResponse::HttpResponse(HttpStatus status)
		: mStatus(status)
	{}

	std::string HttpResponse::GetHeaderString() const
	{
		std::string result = Util::Format("{} {} {}{}", VERSION, mStatus.mCode, mStatus.GetReason(), "\r\n");

		result += mHeader.ToString([](const std::string& key, const std::string&) {
			return true;
		});

		return result;
	}

	const HttpStatus& HttpResponse::GetStatus() const
	{
		return mStatus;
	}

	bool HttpResponse::isValid() const
	{
		return mStatus.isValid();
	}

	bool HttpResponse::ParseHeader(std::string_view rawStr)
	{
		const size_t statusLineEnd = rawStr.find("\r\n");
		if (statusLineEnd == std::string_view::npos) {
			return false;
		}

		const std::string_view statusLine = rawStr.substr(0, statusLineEnd);
		rawStr.remove_prefix(statusLineEnd + 2);

		std::istringstream statusStream{ std::string(statusLine) };
		std::string version;
		size_t statusCode;
		statusStream >> version >> statusCode;
		mStatus = HttpStatus{ statusCode };

		if (statusStream.fail() || version != VERSION || !mStatus.isValid()) {
			return false;
		}

		HttpHeader header;
		header.FromString(rawStr);
		mHeader = std::move(header);

		return true;
	}


}