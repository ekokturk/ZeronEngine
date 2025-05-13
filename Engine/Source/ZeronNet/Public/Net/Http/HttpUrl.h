// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once


namespace Zeron::Net
{
	class HttpUrl {
	  public:
		HttpUrl() = default;
		HttpUrl(const std::string& url);

		bool isValid() const { return mIsValid; }
		const std::string& GetScheme() const { return mScheme; }
		const std::string& GetHost() const { return mHost; }
		size_t GetPort() const { return mPort; }
		const std::string& GetPath() const { return mPath; }
		const std::string& GetQuery() const { return mQuery; }
		const std::string& GetFragment() const { return mFragment; }

		std::string GetFullPath() const;
		std::string ToString() const;

	  private:
		std::string mScheme;
		std::string mHost;
		size_t mPort = 0;
		std::string mPath;
		std::string mQuery;
		std::string mFragment;
		bool mIsValid = false;
	};
}