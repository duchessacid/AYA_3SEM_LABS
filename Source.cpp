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
boost::recursive_mutex mx;
class Client;
vector<shared_ptr<Client>> clients;

class Client {
	ip::tcp::socket sock;
	enum { max_msg = 1024 };
	int already_read;
	char buff[max_msg];
	bool started;
	string username;
	bool clients_changed_;
	boost::posix_time::ptime last_ping;
public:
	Client() : sock(service), clients_changed_(false) {}

	ip::tcp::socket& sock_r() {
		return sock;
	}

	string get_username() const {
		return username;
	}

	void answer_to_client() {
		try {
			read_request();
			process_request();
		}
		catch (boost::system::system_error&)
		{
			stop();
		}
		if (timed_out())
			stop();
	}

	void read_request() {
		if (sock.available())
			already_read += sock.read_some(buffer(buff + already_read, max_msg - already_read));
	}

	void process_request() {
		bool found_enter = find(buff, buff + already_read, '\n') < buff + already_read;
		if (!found_enter)
			return;

		last_ping = boost::posix_time::microsec_clock::local_time();
		size_t pos = find(buff, buff + already_read, '\n') - buff;
		string msg(buff, pos);
		copy(buff + already_read, buff + max_msg, buff);
		already_read -= pos + 1;


		if (msg.find("login ") == 0) on_login(msg);
		else if (msg.find("ping") == 0) on_ping();
		else if (msg.find("ask_clients") == 0) on_clients();
		else cerr << "invalid msg " << msg << endl;
	}

	void on_login(const string& msg) {
		istringstream in(msg);
		in >> username >> username;
		//cout<< username_ <<endl;
		write("login ok\n");
		{
			boost::recursive_mutex::scoped_lock lk(mx);
			for (unsigned i = 0; i < clients.size() - 1; i++)
				clients[i]->update_clients_changed();
		}
	}

	void update_clients_changed() {
		clients_changed_ = true;
	}

	void on_ping() {
		//cout<<clients_changed_<<endl;
		write(clients_changed_ ? "ping client_list_changed\n" : "ping ok\n");
		clients_changed_ = false;
	}

	void on_clients() {
		string msg;
		{
			boost::recursive_mutex::scoped_lock lk(mx);
			for (std::vector<std::shared_ptr<Client>>::const_iterator b = clients.cbegin(), e = clients.cend(); b != e; ++b)
				msg += (*b)->get_username() + " ";
		}
		write("clients " + msg + "\n");
	}

	void write(const std::string& msg) {
		//cout<<msg<<endl;
		sock.write_some(buffer(msg));
	}

	void stop() {
		boost::system::error_code err;
		sock.close(err);
	}

	bool timed_out() const {
		boost::posix_time::ptime now = boost::posix_time::microsec_clock::local_time();
		long long ms = (now - last_ping).total_milliseconds();
		return ms > 5000;
	}
};


void accept_thread() {
	ip::tcp::acceptor acceptor(service, ip::tcp::endpoint(ip::tcp::v4(), 8001));
	while (true) {

		shared_ptr<Client> cl = make_shared<Client>();
		cout << "wait client" << endl;
		acceptor.accept(cl->sock_r());
		cout << "client acepted" << endl;
		boost::recursive_mutex::scoped_lock lock(mx);
		clients.push_back(cl);
	}
}

void handle_clients_thread() {
	while (true) {
		boost::this_thread::sleep(boost::posix_time::millisec(1));
		boost::recursive_mutex::scoped_lock lk(mx);
		for (auto b = clients.begin(); b != clients.end(); ++b)
			(*b)->answer_to_client();

		clients.erase(std::remove_if(clients.begin(), clients.end(),
			boost::bind(&Client::timed_out, _1)), clients.end());
	}
}


int main() {
	boost::thread_group threads;
	threads.create_thread(accept_thread);
	threads.create_thread(handle_clients_thread);
	threads.join_all();

	return 0;
}
