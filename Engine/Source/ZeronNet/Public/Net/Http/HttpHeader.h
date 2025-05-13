// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once


namespace Zeron::Net
{
	class HttpHeader {
	  public:
		enum class Field
		{
			CacheControl,
			Connection,
			ContentType,
			ContentLength,
			Date,
			Host,
		};

		enum class FieldConnection
		{
			KeepAlive,
			Close,
			Upgrade,
		};

		enum class FieldContentType
		{
			PlainText,
			Json,
			EventStream,
			Html,
			Xml,
		};

		enum class FieldCacheControl
		{
			NoCache,
			NoStore,
		};

		using Value = std::unordered_map<std::string, std::string>;

		bool Set(std::string key, std::string value);
		HttpHeader& SetContentLength(size_t length);
		HttpHeader& SetConnection(FieldConnection connection);
		HttpHeader& SetContentType(std::optional<FieldContentType> contentType);
		HttpHeader& SetCacheControl(std::optional<FieldCacheControl> cacheControl);
		HttpHeader& SetDate(std::chrono::system_clock::time_point time);

		std::optional<std::string> Get(const std::string& key) const;
		std::optional<std::string> Get(Field key) const;
		size_t GetContentLength() const;
		FieldConnection GetConnection() const;
		const std::optional<FieldContentType>& GetContentType() const;
		const std::optional<FieldCacheControl>& GetCacheControl() const;

		std::string ToString(const std::function<bool(const std::string&, const std::string&)>& filter = nullptr) const;
		void FromString(std::string_view rawStr, const std::function<bool(const std::string&, const std::string&)>& filter = nullptr);

		static std::optional<Field> GetFieldFrom(std::string_view name);
		static std::string_view GetFieldName(Field field);

	  private:
		FieldConnection mConnection = FieldConnection::Close;
		std::optional<FieldContentType> mContentType;
		std::optional<FieldCacheControl> mCacheControl;
		size_t mContentLength = 0;

		Value mHeaders;
	};
}