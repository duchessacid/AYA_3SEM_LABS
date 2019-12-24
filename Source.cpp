#include <iostream>
#include <vector>
#include <mutex>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/posix_time/posix_time_io.hpp>
#include <boost/date_time/posix_time/posix_time_types.hpp>
#include <boost/thread/thread.hpp>
#include <boost/thread/recursive_mutex.hpp>
#include <boost/asio.hpp>


using namespace std;
using namespace boost::asio;

io_service service;

struct talk_to_svr
{
	talk_to_svr(const string& username1) : sock(service), started(true), username(username1) {}

	void connect(ip::tcp::endpoint ep) {
		sock.connect(ep);
	}

	void loop() {
		write("login " + username + "\n");
		read_answer();
		while (started) {
			write_request();
			read_answer();
			boost::this_thread::sleep(boost::posix_time::millisec(rand() % 4000));
		}
	}
	string username_get() const { return username; }

	void write_request() {
		write("ping\n");
	}
	void read_answer() {
		already_read = read(sock, buffer(buff),
			boost::bind(&talk_to_svr::read_complete, this, _1, _2));
		process_msg();
	}
	void process_msg() {
		string msg(buff, already_read);

		if (msg.find("login ") == 0) on_login(msg);
		else if (msg.find("ping ") == 0) on_ping(msg);
		else if (msg.find("clients ") == 0) on_clients(msg);
		else cerr << "invalid msg " << msg << endl;
	}

	void on_login(const string& msg) {
		cout << msg << endl;
		do_ask_clients();
		cout << endl;
	}

	void on_ping(const string& msg) {
		cout << msg << endl;
		istringstream in(msg);
		string answer;
		in >> answer >> answer;
		if (answer == "client_list_changed")
			do_ask_clients();
	}
	void on_clients(const string& msg) {
		string clients = msg.substr(8);
		cout << username << ", new client list:" << clients << endl;
	}
	void do_ask_clients() {
		write("ask_clients\n");
		read_answer();
	}
	void write(const string& msg) {
		sock.write_some(buffer(msg));
	}

	size_t read_complete(const boost::system::error_code& err, size_t bytes) {
		cout << bytes << endl;
		if (err) return 0;
		bool found = find(buff, buff + bytes, '\n') < buff + bytes;
		return found ? 0 : 1;
	}

private:
	ip::tcp::socket sock;
	enum { max_msg = 1024 };
	int already_read;
	char buff[max_msg];
	bool started;
	string username;
};


void run_client(const string& client_name) {
	ip::tcp::endpoint ep(ip::address::from_string("172.16.80.64"), 8001);
	talk_to_svr client(client_name);
	try {
		client.connect(ep);
		client.loop();
	}
	catch (boost::system::system_error& err) {
		cout << "client terminated " << endl;
	}
}

int main() {
	run_client("Katya");
	



	return 0;
}
