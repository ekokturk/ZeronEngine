// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once


namespace Zeron::Net
{
	enum class HttpSessionType
	{
		Unknown = 0,
		Rest,
		SSE,
	};

	class HttpSession {
	  public:
		virtual ~HttpSession() = default;

		virtual size_t GetId() const = 0;
		virtual bool End() = 0;
	};
}