#pragma once
#include <iostream>
#include <string>
#include <vector>

using namespace std;


//template <typename T>
class stack
{
	vector<string> arr;
	int head;
public:
	stack(int head) {
		this->arr.resize(head);
		this->head = 0;
	}

	void push(string value){
		if (head == arr.capacity()) {
			//cout << sizeof(arr[6]);
			cout << "overflow" << endl;
			return;
		}
		head++;
		arr[head - 1] = value;
	}

	string pop() {
		if (head == 0) {
			return "underflow\n";
		}
		head--;
		return arr[head];
	}

	void print() {
		for (int i = 0; i < head; i++) {
			cout << arr[i] << " ";
		}
		cout << endl;
	}
};


