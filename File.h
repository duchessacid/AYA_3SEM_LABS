#pragma once
#include <iostream>
#include <string>
using namespace std;

class File
{
public:
	string broker;
	string account;
	size_t count;
	string last_date;
	File(string br, string filename);
	friend ostream& operator<<(ostream& out, const File& file);
};

