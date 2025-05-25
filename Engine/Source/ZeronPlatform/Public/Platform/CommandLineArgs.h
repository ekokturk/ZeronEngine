// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

namespace Zeron
{
	class CommandLineArgs {
	  public:
		void Process(int argc, char* argv[]);
		bool HasArg(const std::string& arg) const;
		const std::string& GetArgValue(const std::string& arg) const;
		const std::pair<int, char**>& GetRawArgs() const;

	  private:
		std::pair<int, char**> mRawArgs = std::make_pair(0, nullptr);
		std::map<std::string, std::string> mArguments;
	};
}