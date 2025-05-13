// Copyright (C) Eser Kokturk. All Rights Reserved.

#include <Net/Http/HttpStatus.h>


namespace Zeron::Net
{
	static const std::unordered_map<size_t, std::string> gValidStatus = {
		{ HttpStatus::Code::Unknown, "Unknown" },
		{ HttpStatus::Code::Continue, "Continue" },
		{ HttpStatus::Code::SwitchingProtocols, "Switching Protocols" },
		{ HttpStatus::Code::OK, "OK" },
		{ HttpStatus::Code::Created, "Created" },
		{ HttpStatus::Code::Accepted, "Accepted" },
		{ HttpStatus::Code::NoContent, "No Content" },
		{ HttpStatus::Code::MovedPermanently, "Moved Permanently" },
		{ HttpStatus::Code::Found, "Found" },
		{ HttpStatus::Code::NotModified, "Not Modified" },
		{ HttpStatus::Code::BadRequest, "Bad Request" },
		{ HttpStatus::Code::Unauthorized, "Unauthorized" },
		{ HttpStatus::Code::Forbidden, "Forbidden" },
		{ HttpStatus::Code::NotFound, "Not Found" },
		{ HttpStatus::Code::MethodNotAllowed, "Method Not Allowed" },
		{ HttpStatus::Code::RequestTimeout, "Request Timeout" },
		{ HttpStatus::Code::InternalServerError, "Internal Server Error" },
		{ HttpStatus::Code::NotImplemented, "Not Implemented" },
		{ HttpStatus::Code::BadGateway, "Bad Gateway" },
		{ HttpStatus::Code::ServiceUnavailable, "Service Unavailable" },
		{ HttpStatus::Code::GatewayTimeout, "Gateway Timeout" },
	};

	const std::string& HttpStatus::GetReason(Code code)
	{
		const auto itr = gValidStatus.find(code);
		ZE_ASSERT(itr != gValidStatus.end(), "Expected error code to exist");
		return itr->second;
	}

	bool HttpStatus::IsValidCode(Code code)
	{
		return code != HttpStatus::Unknown && gValidStatus.contains(code);
	}

	HttpStatus::HttpStatus()
		: mCode(Unknown)
	{}

	HttpStatus::HttpStatus(Code code)
		: mCode(code)
	{}

	HttpStatus::HttpStatus(size_t code)
	{
		const auto itr = gValidStatus.find(code);
		if (itr != gValidStatus.end()) {
			mCode = static_cast<Code>(itr->first);
		}
		else {
			mCode = HttpStatus::Code::Unknown;
		}
	}

	HttpStatus& HttpStatus::operator=(Code code)
	{
		const auto itr = gValidStatus.find(code);
		if (itr != gValidStatus.end()) {
			mCode = static_cast<Code>(itr->first);
		}
		else {
			mCode = HttpStatus::Code::Unknown;
		}
		return *this;
	}

	bool HttpStatus::isValid() const
	{
		return HttpStatus::Code::Unknown != mCode;
	}

	const std::string& HttpStatus::GetReason() const
	{
		return GetReason(mCode);
	}
}