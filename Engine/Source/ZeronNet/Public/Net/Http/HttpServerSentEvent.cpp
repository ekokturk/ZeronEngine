// Copyright (C) Eser Kokturk. All Rights Reserved.

#include <Net/Http/HttpServerSentEvent.h>


namespace Zeron::Net
{

	std::string HttpServerSentEvent::ToString() const
	{
		std::string evt;
		if (!mId.empty()) {
			evt += Util::Format("id: {}\n", mId);
		}
		if (!mEvent.empty()) {
			evt += Util::Format("event: {}\n", mEvent);
		}

		size_t start = 0;
		const size_t length = mData.size();
		while (true) {
			const size_t endLine = mData.find('\n', start);
			if (endLine == std::string::npos) {
				evt += Util::Format("data: {}\n", mData.substr(start));
				break;
			}
			evt += Util::Format("data: {}\n", mData.substr(start, endLine - start));
			start = endLine + 1;
			if (start == length) {
				evt += "data:\n";
				break;
			}
		}

		evt += "\n";
		return evt;
	}

	bool HttpServerSentEvent::Parse(std::string_view rawStr)
	{
		mId.clear();
		mEvent.clear();
		mData.clear();

		size_t start = 0;
		bool hadEmptyLine = false;
		while (start < rawStr.size()) {
			size_t end = rawStr.find('\n', start);
			if (end == std::string_view::npos) {
				end = rawStr.size();
			}

			std::string_view line = rawStr.substr(start, end - start);
			if (!line.empty() && line.back() == '\r') {
				line.remove_suffix(1);
			}

			if (line.empty()) {
				break;
			}

			if (line.rfind("id:", 0) == 0) {
				auto id = line.substr(3);
				if (!id.empty() && id.front() == ' ') {
					id.remove_prefix(1);
				}
				mId = id;
			}
			else if (line.rfind("event:", 0) == 0) {
				auto evt = line.substr(6);
				if (!evt.empty() && evt.front() == ' ') {
					evt.remove_prefix(1);
				}
				mEvent = evt;
			}
			else if (line.rfind("data:", 0) == 0) {
				auto data = line.substr(5);
				if (!data.empty() && data.front() == ' ') {
					data.remove_prefix(1);
				}
				if (!hadEmptyLine && !mData.empty() || hadEmptyLine) {
					mData += "\n";
				}
				mData.append(data);
				hadEmptyLine = data.empty();
			}

			start = end + 1;
		}

		return !mData.empty();
	}
}