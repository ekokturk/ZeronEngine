// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

#include <Net/Http/HttpHeader.h>

namespace Zeron::Net
{
	class HttpPayload {
	  public:
		static constexpr char VERSION[] = "HTTP/1.1";

		virtual ~HttpPayload() = default;

		const std::string& GetBody() const { return mBody; }
		HttpHeader& GetHeader() { return mHeader; }
		const HttpHeader& GetHeader() const { return mHeader; }

		void SetBody(std::string body);

		std::string ToString() const;

		virtual bool ParseHeader(std::string_view rawStr) = 0;
		virtual std::string GetHeaderString() const = 0;

	  protected:
		HttpHeader mHeader;
		std::string mBody;
	};


}