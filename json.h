#pragma once
#include <iostream>
#include <string>
#include <any>
#include <map>
#include <vector>
#include <sstream>

using namespace std;

class [[nodiscard]] json
{
	any data;
public:
	json();
	explicit json(const string& s);
	json(any data);
	bool isarray() const;
	bool isobject() const;
	any& operator [](const string& key);
	any& operator [](int index);
	static json parse(const string& s);
	static json parsefile(const string& path);
	static map<string, any> parseob(string& ss);
	static int parsenum(istringstream& s);
	static vector<any> parsear(string& s);
	virtual ~json();
};
