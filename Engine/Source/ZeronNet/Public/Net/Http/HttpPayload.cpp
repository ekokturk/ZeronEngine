// Copyright (C) Eser Kokturk. All Rights Reserved.

#include <Net/Http/HttpPayload.h>


namespace Zeron::Net
{
	void HttpPayload::SetBody(std::string body)
	{
		mBody = std::move(body);
		mHeader.Set("Content-Length", std::to_string(mBody.size()));
	}

	std::string HttpPayload::ToString() const
	{
		return GetHeaderString() + "\r\n" + mBody;
	}

}