// Copyright (C) Eser Kokturk. All Rights Reserved.

#include <Platform/CommandLineArgs.h>

namespace Zeron
{
	void CommandLineArgs::Process(int argc, char* argv[])
	{
		for (int i = 1; i < argc; ++i) {
			const std::string arg = argv[i];
			const std::string::size_type pos = arg.find('=');
			if (pos != std::string::npos) {
				const std::string key = arg.substr(0, pos);
				const std::string value = arg.substr(pos + 1);
				mArguments[key] = value;
			}
		}
	}

	bool CommandLineArgs::HasArg(const std::string& arg) const
	{
		return mArguments.contains(arg);
	}

	const std::string& CommandLineArgs::GetArgValue(const std::string& arg) const
	{
		static const std::string emptyArg;
		const auto itr = mArguments.find(arg);
		return itr != mArguments.cend() ? itr->second : emptyArg;
	}
}