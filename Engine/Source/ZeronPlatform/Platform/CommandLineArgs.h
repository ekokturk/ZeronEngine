// Copyright (C) Eser Kokturk. All Rights Reserved.

#pragma once

namespace Zeron
{
	class CommandLineArgs {
	  public:
		void Process(int argc, char* argv[]);
		bool HasArg(const std::string& arg) const;
		const std::string& GetArgValue(const std::string& arg) const;

	  private:
		std::map<std::string, std::string> mArguments;
	};
}