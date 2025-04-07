// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

namespace Zeron::DB
{
	class Transaction {
	  public:
		virtual ~Transaction() = default;

		virtual void Commit() = 0;
		virtual void Discard() = 0;
		virtual void Put(const std::string& key, const std::string& value) = 0;
		virtual std::string Get(const std::string& key) = 0;
		virtual void Delete(const std::string& key) = 0;
	};


}