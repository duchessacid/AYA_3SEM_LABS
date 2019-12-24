#define LIBASYNC_STATIC
#include <async++.h>
#include <memory>
#include <boost/process.hpp>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/array.hpp>
#include <boost/iostreams/stream.hpp>
#include <boost/log/trivial.hpp>
#include "Parse.h"
#include <boost/log/sinks.hpp>
#include <boost/log/utility/setup.hpp>
#include <boost/log/trivial.hpp>
#include <boost/unordered_map.hpp>
#include <thread>
using namespace std;
using namespace chrono;
using namespace boost::log;
using namespace boost::process;

void create(unique_ptr<ThreadData>& data)
{
	if (Globals::timeout <= 0) {
		return;
	}
	while (Globals::timeout > 0 && (!data || !data->isTerminated)) {
		static const int DATA_TIMEOUT = 50;
		this_thread::sleep_for(milliseconds{ DATA_TIMEOUT });
		Globals::timeout -= DATA_TIMEOUT;
	}
	data->isTerminated = true;
	try {
		data->currentProcess.terminate();
	}
	catch (exception& exception) {
		BOOST_LOG_TRIVIAL(error) << exception.what();
	}
	BOOST_LOG_TRIVIAL(info) << "Timeout";
}

void init()
{
	register_simple_formatter_factory<
		trivial::severity_level,
		char
	>("Severity");
	static const string format = "[%TimeStamp%][%ThreadID%][%Severity%]: %Message%";
	auto sinkFile = add_file_log(
		keywords::file_name = ".build/logs/log_%N.log",
		keywords::rotation_size = 128 * 1024 * 1024,
		keywords::auto_flush = true,
		keywords::format = format
	);
	sinkFile->set_filter(
		trivial::severity >= trivial::trace
	);      

	static const unordered_map<string, trivial::severity_level> CONSOLE_FILTER = {
			{"debug", trivial::debug},
			{"info", trivial::info},
			{"warning", trivial::warning},
			{"error", trivial::error},
	};
	auto sinkConsole = add_console_log(
		cout,
		keywords::format = format
	);
	sinkConsole->set_filter(
		trivial::severity >= CONSOLE_FILTER.at(Globals::loglevel)
	);   
	add_common_attributes();
}

void startProcess(const list<string>& commandArgs, unique_ptr<ThreadData>& data)
{
	if (data && data->isTerminated) {
		return;
	}
	BOOST_LOG_TRIVIAL(info) << "Command: cmake";
	for (const auto& arg : commandArgs) {
		BOOST_LOG_TRIVIAL(info) << "                " << arg;
	}
	BOOST_LOG_TRIVIAL(info) << "\n";

	ipstream stream;
	child child{ search_path("cmake"),
				args(commandArgs),
				std_out > stream };

	if (!data) {
		data = make_unique<ThreadData>(ThreadData{ false, move(child) });
	}
	else {
		data->currentProcess = move(child);
	}
	BOOST_LOG_TRIVIAL(debug) << "Data updated";
	for (string line; data->currentProcess.running() && getline(stream, line);) {
		BOOST_LOG_TRIVIAL(info) << line;
	}
	data->currentProcess.wait();
	auto exitCode = data->currentProcess.exit_code();
	BOOST_LOG_TRIVIAL(info) << "Exit code: " << exitCode;
	if (exitCode != 0) {
		BOOST_LOG_TRIVIAL(error) << "Non zero exit code. Exiting...";
		data->isTerminated = true;
	}
}

int main(int argc, char* argv[])
{
	auto returnCode = programArguments(argc, argv);
	if ( returnCode != 0) {
		return returnCode;
	}

	init();
	unique_ptr<ThreadData> data{ nullptr };
	auto timer = async::spawn(boost::bind(&create, std::ref(data)));

	auto build = async::spawn(
		boost::bind(startProcess, Command::getConfig(), std::ref(data))
	).then(
		boost::bind(startProcess, Command::getBuild(), std::ref(data))
	);

	if (Globals::is_install_enabled) {
		build = build.then(
			boost::bind(startProcess, Command::getInstall(), std::ref(data))
		);
	}
	if (Globals::is_pack_enabled) {
		build = build.then(
		boost::bind(startProcess, Command::getPack(), std::ref(data))
		);
	}
	build = build.then([&data]() {
	data->isTerminated = true;
	});

	build.wait();
	timer.wait();
	system("pause");
	return 0;
}
