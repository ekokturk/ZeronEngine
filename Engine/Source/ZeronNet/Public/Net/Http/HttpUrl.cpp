// Copyright (C) Eser Kokturk. All Rights Reserved.

#include <Net/Http/HttpUrl.h>


namespace Zeron::Net
{
	HttpUrl::HttpUrl(const std::string& url)
	{
		// Relative
		if (url.empty() || url[0] == '/') {
			mScheme = "http";
			mHost = "localhost";
			mPort = 0;
			std::string_view view(url);
			const size_t hashPos = view.find('#');
			if (hashPos != std::string_view::npos) {
				mFragment = std::string(view.substr(hashPos + 1));
				view = view.substr(0, hashPos);
			}
			const size_t queryPos = view.find('?');
			if (queryPos != std::string_view::npos) {
				mQuery = std::string(view.substr(queryPos + 1));
				view = view.substr(0, queryPos);
			}

			mPath = view.empty() ? "/" : std::string(view);

			mIsValid = true;
			return;
		}

		// Full
		static const std::regex urlRegex(R"(^(?:(http|https)://)?([^:/?#]+)(?::(\d+))?([^?#]*)?(?:\?([^#]*))?(?:#(.*))?$)", std::regex::icase);
		std::smatch matches;
		if (std::regex_match(url, matches, urlRegex)) {
			mScheme = matches[1].matched ? matches[1].str() : "http";
			mHost = matches[2];
			mPort = matches[3].matched ? std::stoul(matches[3]) : (mScheme == "https" ? 443 : 80);
			mPath = matches[4].matched && !matches[4].str().empty() ? matches[4].str() : "/";
			mQuery = matches[5].matched ? matches[5].str() : "";
			mFragment = matches[6].matched ? matches[6].str() : "";
			mIsValid = true;
		}
	}

	std::string HttpUrl::GetFullPath() const
	{
		std::string fullPath = GetPath();
		if (!GetQuery().empty()) {
			fullPath += "?" + GetQuery();
		}
		return fullPath;
	}

	std::string HttpUrl::ToString() const
	{
		std::ostringstream oss;
		oss << mScheme << "://" << mHost;
		if ((mScheme == "http" && mPort != 80) || (mScheme == "https" && mPort != 443)) {
			oss << ":" << mPort;
		}
		oss << mPath;
		if (!mQuery.empty()) {
			oss << "?" << mQuery;
		}
		if (!mFragment.empty()) {
			oss << "#" << mFragment;
		}
		return oss.str();
	}
}