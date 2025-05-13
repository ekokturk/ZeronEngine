// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

namespace Zeron::Net
{
	class HttpServerSentEvent {
	  public:
		std::string ToString() const;
		bool Parse(std::string_view rawStr);

		std::string mId;
		std::string mEvent;
		std::string mData;
	};

	using HttpServerSentEventCallback = std::function<void(const HttpServerSentEvent* evt, std::error_code err)>;
}