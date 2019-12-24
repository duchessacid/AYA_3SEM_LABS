#include <iostream>
#include <cstdlib>
#include "picosha2.h"
#include <thread>
#include <vector>
#include <string>
#include <boost/lexical_cast.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/core.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/console.hpp>
namespace logging = boost::log;
using namespace std;

const string end_hash = "0000";
string chars = "abcdefghijklmnaoqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890";

void search() {
	unsigned int indexes[10];
	string random_string = "";
	while (1) {
		unsigned int length = rand() % 10 + 1;
		for (unsigned int i = 0; i < length; ++i) {
			indexes[i] = rand() % chars.length();
		}
		for (unsigned int i = 0; i < length; ++i)
		{
			random_string += chars[indexes[i]];
		}
		string hash_string = picosha2::hash256_hex_string(random_string);
		if (hash_string.substr(hash_string.size() - end_hash.size()) == end_hash)
		{
			BOOST_LOG_TRIVIAL(info) << "Found correct value '" << random_string << "' with hash '" << hash_string << "'";
		}
		else
		{
			BOOST_LOG_TRIVIAL(trace) << "Thread with ID: " << this_thread::get_id() << " || Current value '" << random_string << "' with hash '" << hash_string << "'";
		}
		random_string.clear();
	}
}

int main(int argc, char* argv[]) {
	vector <thread> threads;
	size_t numb_thr = 0;
	if (argv[1]) numb_thr = boost::lexical_cast<size_t>(argv[1]);
	else {
		numb_thr = boost::lexical_cast<size_t>(thread::hardware_concurrency());
	}
	threads.reserve(numb_thr);
	logging::add_file_log("log.log");
	srand(time(NULL));
	logging::add_console_log(cout);
	for (size_t i = 0; i < numb_thr; i++)
		{
			threads.emplace_back(thread(search));
		}

	for (thread& thread : threads){
		thread.join();
	}
	system("pause");
	return 0;
}
