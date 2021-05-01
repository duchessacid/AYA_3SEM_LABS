#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <fstream>
using namespace std;

class queue
{
	string* arr;
	int max_size;
	int head;
	int tail;
public:
	queue(int size) {
		this->arr = new string[size];
		this->max_size = size;
		this->head = 0;
		this->tail = -1;
	}

	~queue() {
		delete[] arr;
	}
	
	void push(string x, ofstream& out) {
		if (((head - tail == 1) && (tail != -1)) || (tail - head == max_size - 1)) {
			out << "overflow\n";
			return;
		}
		if ((tail + 1) == max_size) tail = 0;
		else {
			tail++;
		}
		arr[tail] = x;
	}

	string pop() {
		if (tail == -1){
			return "underflow";
		}
		string x = arr[head];
		if (head == tail) { head = 0; tail = -1; }
		else if ((head + 1) == max_size) head = 0;
		else {
			head++;
		}
		return x;
	}

	void print(ofstream& out) {
		if (tail == -1) out << "empty";
		else if (tail >= head) {
			for (int i = head; i <= tail; i++) {
				if (i <= tail - 1) out << arr[i] << " ";
				else {
					out << arr[i];
				}
			}
		}
		else {
			for (int i = head; i < max_size; i++) { out << arr[i] << " "; }
			for (int i = 0; i <= tail; i++) {
				if (i <= tail - 1) out << arr[i] << " ";
				else {
					out << arr[i];
				}
			}
		}
		out << endl;
	}
};


int main(int argc, char* argv[]) {
	stringstream ss;
	string command, val, str;
	int int_val = 0;
	string check = "correct";
	ifstream input;
	ofstream output;
	if (argc == 3) {
		input.open(argv[1]);
		output.open(argv[2]);
	}
	else {
		cout << "error" << endl;
	}
	while (getline(input, str)) {
		if (str.empty()) continue;
		ss << str;
		ss >> command >> val >> check;
		if (check != "correct") {
			output << "error" << endl;
			check = "correct";

		}
		else if (command == "set_size") {
			int_val = stoi(val);
			ss.clear();
			str.clear();
			val.clear();
			break;
		}
		else {
			output << "error" << endl;
		}
		ss.clear();
		str.clear();
		val.clear();
	}

	queue Q(int_val);
	while (getline(input, str)) {
		if (str.empty()) continue;
		ss << str;
		ss >> command >> val >> check;
		if (check != "correct") {
			output << "error" << endl;
			check = "correct";

		}
		else if (command == "push") {
			Q.push(val, output);
		}
		else if (command == "pop") {
			if ((!val.empty()) || (str.find(" ") != string::npos)) output << "error" << endl;
			else {
				output << Q.pop() << endl;
			}
		}
		else if (command == "print") {
			if ((!val.empty()) || (str.find(" ") != string::npos)) output << "error" << endl;
			else {
				Q.print(output);
			}
		}
		else {
			output << "error" << endl;
		}
		ss.clear();
		str.clear();
		val.clear();
	}
	input.close();
	output.close();
	return 0;
}