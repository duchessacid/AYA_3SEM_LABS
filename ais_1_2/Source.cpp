#include <iostream>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

class stack
{
	string* arr;
	int max_size;
	int head;
public:
	stack(int size) {
		this->max_size = size;
		this->arr = new string[size];
		this->head = 0;
	}

	~stack(){
		delete[] arr;
	}

	void push(string value) {
		if (head == max_size) {
			cout << "overflow" << endl;
			return;
		}
		head++;
		arr[head - 1] = value;
	}

	string pop() {
		if (head == 0) {
			return "underflow";
		}
		head--;
		return arr[head];
	}

	void print() {
		if (head == 0) cout << "empty" << endl;
		else {
			for (int i = 0; i < head; i++) {
				if (i < head - 1) cout << arr[i] << " ";
				else {
					cout << arr[i];
				}
			}
			cout << endl;
		}
	}
};

void parse() {
	int int_val = 0;
	string str, command, val;
	string check = "correct";
	stringstream ss;
	int flag = 0;
	while (getline(cin, str)) {
		if (str.empty()) continue;
		ss << str;
		ss >> command >> val >> check;
		if (check != "correct") {
			cout << "error" << endl;
			check = "correct";
		}
		else if (command == "set_size") {
			for (int i = 0; i <= val.length(); i++) {
				if ((val[i] >= '0') && (val[i] <= '9')) {
					flag++;
				}
			}
			if (flag == val.length()) {
				int_val = stoi(val);
				ss.clear();
				str.clear();
				val.clear();
				break;
			}
			else { cout << "error" << endl; }
		}
		else {
			cout << "error" << endl;
		}
		ss.clear();
		str.clear();
		val.clear();
	}

	stack St(int_val);
	while (getline(cin, str)) {
		if (str.empty()) continue;
		ss << str;
		ss >> command >> val >> check;
		if (check != "correct") {
			cout << "error" << endl;
			check = "correct";
		}
		else if (command == "push") {
			St.push(val);
		}
		else if (command == "pop") {
			if ((!val.empty()) || (str.find(" ") != string::npos)) cout << "error" << endl;
			else {
				cout << St.pop() << endl;
			}
		}
		else if (command == "print") {
			if ((!val.empty()) || (str.find(" ") != string::npos)) cout << "error" << endl;
			else {
				St.print();
			}
		}
		else {
			cout << "error" << endl;
		}
		ss.clear();
		str.clear();
		val.clear();
	}
}

int main() {
	parse();
	return 0;
}