// Copyright (C) Eser Kokturk. All Rights Reserved.

#include <Net/Http/HttpHeader.h>


namespace Zeron::Net
{
	namespace
	{
#define REVERSE_LOOKUP(varName, otherMap)                  \
	static const auto& varName = []() {                    \
		using Key = decltype((otherMap).begin()->second);  \
		using Value = decltype((otherMap).begin()->first); \
		static std::unordered_map<Key, Value> reversed;    \
		for (const auto& [key, value] : (otherMap)) {      \
			reversed.emplace(value, key);                  \
		}                                                  \
		return reversed;                                   \
	}();


		const std::unordered_map<HttpHeader::Field, std::string> FieldToString{
			{ HttpHeader::Field::CacheControl, "Cache-Control" },
			{ HttpHeader::Field::Connection, "Connection" },
			{ HttpHeader::Field::ContentType, "Content-Type" },
			{ HttpHeader::Field::ContentLength, "Content-Length" },
			{ HttpHeader::Field::Date, "Date" },
			{ HttpHeader::Field::Host, "Host" },
		};

		REVERSE_LOOKUP(FieldFromString, FieldToString);

		const std::unordered_map<HttpHeader::FieldConnection, std::string> ConnectionToString{
			{ HttpHeader::FieldConnection::KeepAlive, "keep-alive" },
			{ HttpHeader::FieldConnection::Close, "close" },
			{ HttpHeader::FieldConnection::Upgrade, "upgrade" },
		};

		REVERSE_LOOKUP(ConnectionFromString, ConnectionToString);

		const std::unordered_map<HttpHeader::FieldContentType, std::string> ContentTypeToString{
			{ HttpHeader::FieldContentType::PlainText, "text/plain" },			{ HttpHeader::FieldContentType::Html, "text/html" },
			{ HttpHeader::FieldContentType::Json, "application/json" },			{ HttpHeader::FieldContentType::Xml, "application/xml" },
			{ HttpHeader::FieldContentType::EventStream, "text/event-stream" },
		};

		REVERSE_LOOKUP(ContentTypeFromString, ContentTypeToString);

		const std::unordered_map<HttpHeader::FieldCacheControl, std::string> CacheControlToString{
			{ HttpHeader::FieldCacheControl::NoCache, "no-cache" },
			{ HttpHeader::FieldCacheControl::NoStore, "no-store" },
		};

		REVERSE_LOOKUP(CacheControlFromString, CacheControlToString);
	}

	bool HttpHeader::Set(std::string key, std::string value)
	{
		if (const std::optional<Field> field = GetFieldFrom(key)) {
			switch (field.value()) {
				case Field::CacheControl: {
					if (auto itr = CacheControlFromString.find(value); itr != CacheControlFromString.end()) {
						SetCacheControl(itr->second);
						return true;
					}
				} break;
				case Field::Connection: {
					if (auto itr = ConnectionFromString.find(value); itr != ConnectionFromString.end()) {
						SetConnection(itr->second);
						return true;
					}
				} break;
				case Field::ContentType: {
					if (auto itr = ContentTypeFromString.find(value); itr != ContentTypeFromString.end()) {
						SetContentType(itr->second);
						return true;
					}
				} break;
				case Field::ContentLength: {
					size_t contentLength = 0;
					std::stringstream ss(value);
					ss >> contentLength;
					if (!ss.fail() && ss.eof()) {
						mContentLength = contentLength;
						return true;
					}
				} break;
			}
		}
		mHeaders.emplace(std::move(key), std::move(value));
		return true;
	}

	HttpHeader& HttpHeader::SetContentLength(size_t length)
	{
		mContentLength = length;
		return *this;
	}

	HttpHeader& HttpHeader::SetConnection(FieldConnection connection)
	{
		mConnection = connection;
		return *this;
	}

	HttpHeader& HttpHeader::SetContentType(std::optional<FieldContentType> contentType)
	{
		mContentType = contentType;
		return *this;
	}

	HttpHeader& HttpHeader::SetCacheControl(std::optional<FieldCacheControl> cacheControl)
	{
		mCacheControl = cacheControl;
		return *this;
	}

	HttpHeader& HttpHeader::SetDate(std::chrono::system_clock::time_point time)
	{
		// TODO: Move this to time library
		// TODO: Store this as time point
		std::time_t t = std::chrono::system_clock::to_time_t(time);
		std::tm gm;
#if defined(_WIN32)
		gmtime_s(&gm, &t);
#else
		gmtime_r(&t, &gm);
#endif
		mHeaders.emplace(std::string(GetFieldName(Field::Date)), Util::Format("{:%a, %d %b %Y %H:%M:%S GMT}", gm));
		return *this;
	}

	std::optional<std::string> HttpHeader::Get(const std::string& key) const
	{
		if (const std::optional<Field> field = GetFieldFrom(key)) {
			if (std::optional<std::string> fieldVal = Get(*field)) {
				return fieldVal;
			}
		}
		auto it = mHeaders.find(key);
		if (it != mHeaders.end()) {
			return it->second;
		}
		return std::nullopt;
	}

	std::optional<std::string> HttpHeader::Get(Field key) const
	{
		switch (key) {
			case Field::Connection: {
				return ConnectionToString.at(mConnection);
			} break;
			case Field::ContentType: {
				if (mContentType.has_value()) {
					return ContentTypeToString.at(*mContentType);
				}
			} break;
			case Field::CacheControl: {
				if (mContentType.has_value()) {
					return ContentTypeToString.at(*mContentType);
				}
			} break;
			case Field::ContentLength: {
				return std::to_string(mContentLength);
			} break;
			case Field::Date:
			case Field::Host: {
				auto it = mHeaders.find(std::string(GetFieldName(key)));
				if (it != mHeaders.end()) {
					return it->second;
				}
			} break;
			default: ZE_FAIL("HttpHeader: Field getter is not implemented");
		}

		return std::nullopt;
	}

	size_t HttpHeader::GetContentLength() const
	{
		return mContentLength;
	}

	HttpHeader::FieldConnection HttpHeader::GetConnection() const
	{
		return mConnection;
	}

	const std::optional<HttpHeader::FieldContentType>& HttpHeader::GetContentType() const
	{
		return mContentType;
	}

	const std::optional<HttpHeader::FieldCacheControl>& HttpHeader::GetCacheControl() const
	{
		return mCacheControl;
	}

	std::string HttpHeader::ToString(const std::function<bool(const std::string&, const std::string&)>& filter) const
	{
		std::string str;
		constexpr std::string_view headerFormat = "{}: {}\r\n";
		for (const auto& [key, value] : mHeaders) {
			if (filter && !filter(key, value)) {
				continue;
			}
			str += Util::Format(headerFormat, key, value);
		}

		str += Util::Format(headerFormat, GetFieldName(Field::ContentLength), mContentLength);
		if (mContentType.has_value()) {
			str += Util::Format(headerFormat, GetFieldName(Field::ContentType), ContentTypeToString.at(*mContentType));
		}
		if (mCacheControl.has_value()) {
			str += Util::Format(headerFormat, GetFieldName(Field::CacheControl), CacheControlToString.at(*mCacheControl));
		}
		str += Util::Format(headerFormat, GetFieldName(Field::Connection), ConnectionToString.at(mConnection));

		return str;
	}

	void HttpHeader::FromString(std::string_view rawStr, const std::function<bool(const std::string&, const std::string&)>& filter)
	{
		while (!rawStr.empty()) {
			const size_t lineEnd = rawStr.find("\r\n");
			std::string_view line = (lineEnd == std::string_view::npos) ? rawStr : rawStr.substr(0, lineEnd);

			if (lineEnd != std::string_view::npos) {
				rawStr.remove_prefix(lineEnd + 2);
			}
			else {
				rawStr = {};
			}

			if (line.empty()) {
				continue;
			}

			const size_t colonPos = line.find(':');
			if (colonPos == std::string_view::npos) {
				continue;
			}

			std::string_view key = line.substr(0, colonPos);
			std::string_view value = line.substr(colonPos + 1);

			const size_t start = value.find_first_not_of(" \t");
			if (start != std::string_view::npos) {
				value.remove_prefix(start);
			}

			std::string fieldName(key);
			std::string fieldVal(value);

			if (!filter || filter(fieldName, fieldVal)) {
				Set(std::move(fieldName), std::move(fieldVal));
			}
		}
	}

	std::string_view HttpHeader::GetFieldName(Field field)
	{
		return FieldToString.at(field);
	}

	std::optional<HttpHeader::Field> HttpHeader::GetFieldFrom(std::string_view name)
	{
		auto itr = FieldFromString.find(std::string(name));
		if (itr != FieldFromString.end()) {
			return itr->second;
		}
		return std::nullopt;
	}
}