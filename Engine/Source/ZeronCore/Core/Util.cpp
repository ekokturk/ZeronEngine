// Copyright (C) Eser Kokturk. All Rights Reserved.

#include <Core/Util.h>

#include <base64.h>
#include <sha.h>

namespace Zeron
{
	std::string Util::EncodeBase64(std::string_view str)
	{
		std::string out;
		CryptoPP::Base64Encoder encoder(new CryptoPP::StringSink(out), false);
		encoder.Put(reinterpret_cast<const CryptoPP::byte*>(str.data()), str.size());
		encoder.MessageEnd();
		return out;
	}

	std::string Util::DecodeBase64(std::string_view str)
	{
		std::string out;
		CryptoPP::Base64Decoder decoder(new CryptoPP::StringSink(out));
		decoder.Put(reinterpret_cast<const CryptoPP::byte*>(str.data()), str.size());
		decoder.MessageEnd();
		return out;
	}

	std::string Util::GenerateSHA1(std::string_view str)
	{
		std::string out;
		CryptoPP::SHA1 hash;
		CryptoPP::HashFilter filter(hash, new CryptoPP::StringSink(out));
		filter.Put(reinterpret_cast<const CryptoPP::byte*>(str.data()), str.size());
		filter.MessageEnd();
		return out;
	}
}