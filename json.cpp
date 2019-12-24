#include "json.h"
#include <iostream>
#include <string>
#include <any>
#include <map>
#include <vector>
#include <sstream>
#include <fstream>

using namespace std;

json::json(const string &s) {
	parse(s);
}

json::json() {
}
json::json(any data) : data(data) {
}

bool json::isarray() const {
	try {
		vector<any> c = any_cast<vector<any>>(data);
	}
	catch (bad_any_cast&) {
		return false;
	}
	return true;
}

bool json::isobject() const {
	try {
		map<string, any> c = any_cast<map<string, any>>(data);
	}
	catch (bad_any_cast&) {
		return false;
	}
	return true;
}

enum State {
	findbend,
	findcol,
	findval,
	findcend
} state;

vector<any> json::parsear(string& ss) {
	vector <any> obj;
	string s = ss;
	for (size_t i = 0; i < s.length(); i++) {
		switch (s[i]) {
		case '[':
			state = findval;
			break;
		case ' ':
			break;
		case ',':
			state = findval;
			break;
		case ']':
			return obj;
			break;
		case 'f': case 't': {
			if (state == findval) {
				bool v;
				if (s[i] == 'f') v = false;
				else if (s[i] == 't') v = true;
				obj.push_back(v);
				state = findbend;
			}
			break;
		}
		case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9': case '0': {
			if (state == findval) {
				int v;
				string val;
				while (s[i] != ',') {
					val += s[i];
					i++;
				}
				istringstream s1(val);
				v = parsenum(s1);
				obj.push_back(v);
				state = findbend;
			}
		}
		default:
			if (state == findval) {
				string val;
				while (!(s[i] == ',' || s[i] == ']')) {
					val += s[i];
					++i;
				}
				obj.push_back(val);
				val.clear();
			}
			break;
		}
	}
}
int json::parsenum(istringstream& s) {
	int v1;
	while (s >> v1) {
		return v1;
	}
}

map<string, any> json::parseob(string& ss) {
	map <string, any> object;
	state = findbend;
	string str = ss;
	string key, value;
	for (size_t i = 1; i < str.length(); i++) {
		switch (str[i]) {
		case ' ': case '/n':
			break;
		case '"': {
			if (state == findbend) {
				++i;
				while (str[i] != '"') {
					key += str[i];
					++i;
				}
				state = findcol;
			}
			else if (state == findval) {
				++i;
				while (str[i] != '"') {
					value += str[i];
					++i;
				}
				object.insert(pair <string, any>(key, value));
				key.clear();
				value.clear();
				state = findbend;
			}
			break;
		}
		case ':':
			state = findval;
			break;
		case '}': {
			return object;
			break;
		}
		case '{': {
			map <string, any> object1;
			string s;
			while (str[i] != '}') {
				s += str[i];
				++i;
			}
			s += ',';
			s += '}';
			object1 = parseob(s);
			any j = object1;
			json jj(j);
			object.insert(pair <string, any>(key, jj));
			key.clear();
			object1.clear();
			break;
		}
		case '[': {
			vector <any> obj;
			string s;
			while (str[i] != ']') {
				s += str[i];
				i++;
			}
			//s += ',';
			s += ']';
			obj = parsear(s);
			object.insert(pair <string, any>(key, obj));
			key.clear();

			break;
		}
		case 'f': case 't':
			if (state == findval) {
				bool v;
				if (str[i] == 'f') v = false;
				else if (str[i] == 't') v = true;
				object.insert(pair <string, any>(key, v));
				state = findbend;
				key.clear();
				value.clear();
			}
			break;
		case ',':
			state = findbend;
			break;
		case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9': case '0':
			if (state == findval) {
				int v;
				while (str[i] != ',') {
					value += str[i];
					i++;
				}
				istringstream s(value);
				v = parsenum(s);
				object.insert(pair <string, any>(key, v));
				state = findbend;
				key.clear();
				value.clear();
			}
		}

	}
}

json json::parse(const string& s) {
	any d;
	string ss = s;
	if (s[0] == '{') {
		map <string, any> m = parseob(ss);
		d = m;
	}
	else {
		vector <any> v = parsear(ss);
		d = v;
	}
	json js(d);
	return js;
}

json json::parsefile(const string& path) {
	ifstream fin(path);
	string js;
	if (!fin) cout << "file not found";
	while (fin) {
		string str;
		getline(fin, str);
		js += str + '\n';
	}
	fin.close();
	return json::parse(js);
}


any& json::operator[](const string& key) {
	if (isobject()) {
		//any ref = any_cast<map<string, any>>(data).at(key);
		return any_cast<map<string, any>&>(data)[key];
	}
	else {
		throw logic_error("This is not an object!");
	}
}

any& json::operator[](int index) {
	if (isarray()) {
		auto m = any_cast<vector<any>>(data);
		return m[index];
		//return d;
	}
	else {
		throw logic_error("This is not an array!");
	}
}

json::~json()
{
}
