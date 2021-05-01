#pragma once
#include <iostream>
#include <vector>
#include <string>
using namespace std;

class queue
{
	vector<string> arr;
	int head;
	int tail;
public:
	queue(int tail) {
		this->arr.resize(tail);
		this->head = 0;
		this->tail = 0;
	}
	
	void push(string x) {
		if (tail - head >= arr.capacity()) {
			cout << "overflow" << endl;
			return;
		}
		arr[tail] = x;
		tail++;
	}

	string pop() {
		if (head == tail) {
			return "underflow\n";
		}
		string x = arr[head];
		head++;
		return x;
	}

	void print() {
		for (int i = head; i < tail; i++) {
			cout << arr[i] << " ";
		}
		cout << endl;
	}
};

