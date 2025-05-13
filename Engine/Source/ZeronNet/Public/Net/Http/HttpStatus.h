// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once


namespace Zeron::Net
{
	class HttpStatus {
	  public:
		enum Code : uint16_t
		{
			Unknown = 0,
			Continue = 100,
			SwitchingProtocols = 101,
			OK = 200,
			Created = 201,
			Accepted = 202,
			NoContent = 204,
			MovedPermanently = 301,
			Found = 302,
			NotModified = 304,
			BadRequest = 400,
			Unauthorized = 401,
			Forbidden = 403,
			NotFound = 404,
			MethodNotAllowed = 405,
			RequestTimeout = 408,
			InternalServerError = 500,
			NotImplemented = 501,
			BadGateway = 502,
			ServiceUnavailable = 503,
			GatewayTimeout = 504
		};

		static const std::string& GetReason(Code code);
		static bool IsValidCode(Code code);

		HttpStatus();
		HttpStatus(Code code);
		HttpStatus(size_t code);
		HttpStatus& operator=(const HttpStatus&) = default;
		HttpStatus& operator=(Code code);
		bool isValid() const;
		explicit operator HttpStatus::Code() const { return mCode; }
		const std::string& GetReason() const;

		Code mCode;
	};
}