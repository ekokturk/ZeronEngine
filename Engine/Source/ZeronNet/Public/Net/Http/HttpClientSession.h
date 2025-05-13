// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

#include <Net/Http/HttpSession.h>

namespace Zeron::Net
{
	class HttpClientSession : public HttpSession {
	  public:
		virtual ~HttpClientSession() = default;

		virtual void Start() = 0;
		virtual bool IsAlive() const = 0;
	};
}