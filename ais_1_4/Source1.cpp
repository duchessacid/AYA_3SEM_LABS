#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <deque>
#include <map>
#include <set>
#include <algorithm>

using namespace std;
using Graph = map<string, set<string>>;

void add_d(Graph& g, string i, string j) {
	g[i].insert(j);
}

void add_u(Graph& g, string i, string j) {
	add_d(g, i, j);
	add_d(g, j, i);
}

void search_d(Graph& g, const string& start_v, vector<string>& visited) {
	visited.push_back(start_v);
	for (const auto& i : g[start_v]) {
		if (find(visited.begin(), visited.end(), i) == visited.end()) search_d(g, i, visited);
	}
}

void search_b(Graph& g, const string& start_v, vector<string>& visited) {
	deque<string> help;
	help.push_back(start_v);
	visited.push_back(start_v);
	do {
		string curr_v = help.front();
		help.pop_front();
		for (const auto& i : g[curr_v]) {
			if (find(visited.begin(), visited.end(), i) == visited.end()) {
				visited.push_back(i);
				help.push_back(i);
			}
		}
	} while (help.size() > 0);
}


void parse() {
	string str, g_type, s_type;
	string start_v, fin_v, beg_v;
	stringstream ss;
	bool flag = 0;
	Graph g;
	while (getline(cin, str)) {
		if (str.empty()) continue;
		ss << str;
		if (!flag) {
			ss >> g_type >> start_v >> s_type;
			flag = 1;
		}
		else {
			ss >> beg_v >> fin_v;
			if (g_type == "d") {
				add_d(g, beg_v, fin_v);
			}
			else if (g_type == "u") {
				add_u(g, beg_v, fin_v);
			}
		}
		ss.clear();
		str.clear();
	}

	if (s_type == "d") {
		vector<string>visited;
		search_d(g, start_v, visited);
		for (const auto& i : visited)
		{
			cout << i << endl;
		}
	}

	else if (s_type == "b") {
		vector<string>visited;
		search_b(g, start_v, visited);
		for (const auto& i : visited)
		{
			cout << i << endl;
		}

	}
}

int main() {
	parse();
	return 0;
}