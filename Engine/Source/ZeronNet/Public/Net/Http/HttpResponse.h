// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

#include <Net/Http/HttpPayload.h>
#include <Net/Http/HttpStatus.h>

namespace Zeron::Net
{
	class HttpResponse : public HttpPayload {
	  public:
		HttpResponse();
		HttpResponse(HttpStatus status);

		bool ParseHeader(std::string_view rawStr) override;
		std::string GetHeaderString() const override;
		const HttpStatus& GetStatus() const;

		bool isValid() const;

	  private:
		HttpStatus mStatus;
	};

	using HttpResponseError = std::error_code;
	using HttpResponseCallback = std::function<void(std::optional<HttpResponse>, const HttpResponseError&)>;
}