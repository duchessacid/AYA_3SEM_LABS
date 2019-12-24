#pragma once
#include <string>
#include <list>
#include <boost/filesystem.hpp>
#include <boost/process.hpp>
#include "process.hpp"
#include <random>
#include <iostream>
#include <boost/program_options.hpp>
#include <boost/program_options/parsers.hpp>

using namespace boost::process;
using namespace boost::filesystem;
using namespace boost::program_options;
using namespace std;


struct Globals
{
	static string loglevel;
	static int timeout;
	static bool is_install_enabled;
	static bool is_pack_enabled;
	static string build_config;
};

string Globals::loglevel{};
int Globals::timeout{};
bool Globals::is_install_enabled{};
bool Globals::is_pack_enabled{};
string Globals::build_config{};

class Command
{
public:
	static const string DIRECTORY_BUILD;
	static const string DIRECTORY_INSTALL;
	static list<string> getConfig();
	static list<string> getBuild();
	static list<string> getInstall();
	static list<string> getPack();
};

const string Command::DIRECTORY_BUILD = ".build";
const string Command::DIRECTORY_INSTALL = ".install";

list<string> Command::getConfig()
{
	return { "-H.",
			"-B" + DIRECTORY_BUILD,
			+"-DCMAKE_INSTALL_PREFIX=" + DIRECTORY_INSTALL,
			+"-DCMAKE_BUILD_TYPE=" + Globals::build_config 
	};
}

list<string> Command::getBuild()
{
	return { "--build", DIRECTORY_BUILD };
}

list<string> Command::getInstall()
{
	return { "--build", DIRECTORY_BUILD,
			"--target", "install" 
	};
}

list<string> Command::getPack()
{
	return { "--build", DIRECTORY_BUILD,
			"--target", "package"
	};
}

struct ThreadData
{
	ThreadData() = delete;
	bool isTerminated = false;
	child currentProcess;
};

int programArguments(int argc, char** argv)
{
	positional_options_description positionalArgs;
	positionalArgs.add("input", -1);
	options_description visibleOptions("Available options");
	visibleOptions.add_options()
		("log-level",
			value<string>(&Globals::loglevel)->default_value("info"),
			"info, warning or error")
			("config",
				value<string>(&Globals::build_config)->default_value("Debug"),
				"DCMAKE_BUILD_TYPE")
				("install",
					"Add install step")
					("pack",
						"Add pack step")
						("timeout",
							value<int>(&Globals::timeout)->default_value(-1),
							"Process timeout (ms)")
							("help", "Prints help message");

	variables_map variablesMap;
	store(parse_command_line(argc, argv, visibleOptions), variablesMap);
	notify(variablesMap);
	if (variablesMap.count("help")) {
		cout << visibleOptions << "\n";
		return 1;
	}
	if (variablesMap.count("install")) {
		Globals::is_install_enabled = true;
	}
	if (variablesMap.count("pack")) {
		Globals::is_pack_enabled = true;
	}
	return 0;
}
