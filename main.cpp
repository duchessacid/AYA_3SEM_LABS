#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <boost/filesystem.hpp>
#include "File.h"
using namespace boost::filesystem;
using namespace std;

void parsefile(vector<path> p, vector <File> f) {
	for (auto& i : p) {
		if (is_regular_file(i)) {
			string filename = i.filename().string();
			if (filename.length() != 29) continue;
			if (filename.substr(0, 8) != "balance_") continue;
			for (size_t i = 8; i < 16; i++) {
				if (filename[i] < '0' || filename[i] > '9') continue;
			}
			if (filename[16] != '_') continue;
			for (size_t i = 17; i < 25; i++) {
				if (filename[i] < '0' || filename[i] > '9') continue;
			}
			cout << i.parent_path().filename().string() << " " << filename << '\n';
			File new_file(i.parent_path().filename().string(), filename);
			bool inserted = false;
			for (auto& pos : f) {
				if (pos.broker == new_file.broker) {
					if (pos.account == new_file.account) {
						pos.count++;
						pos.last_date = pos.last_date > new_file.last_date
							? pos.last_date
							: new_file.last_date;
						inserted = true;
					}
				}
			}
			if (!inserted) {
				f.push_back(new_file);
			}
		}
	}
	for (auto pos : f) {
		cout << endl << pos << endl;
	}
}

int main(int argc, char* argv[]) {
	string filename;
	if (argc < 2)
	{
		path p(current_path());
	}
	path p(argv[1]);
	vector<File>files;
	for (const directory_entry& x : directory_iterator(p))
	{
		if (is_directory(x)) {
			vector<path> v;
			for (auto&& i : directory_iterator(x)) {
				v.push_back(i.path());
			}
			sort(v.begin(), v.end());
			parsefile(v, files);
		}
	}
	system("pause");
	return 0;
}
