#include <iostream>
#include <vector>
#include <string>
using namespace std;


int main(){
	long long int S = 0;
	string x, res;
	while (cin >> x){
		for (int i = 0; i < x.length(); i++) {
			if ((x[i] >= '0') && (x[i] <= '9') || ((x[i] == '-') && (x[i + 1] >= '0') && (x[i + 1] <= '9'))) {
				res += x[i];
				if (res == "--") res = "-";
			}
			else if ((res.empty()) || (res == "-") || (res == "--")) continue;
			else if (!res.empty()){
				S += stoi(res);
				res.clear();
			}
		}
		if (!res.empty()) {
			S += stoi(res);
			res.clear();
		}
	}
	//cin.clear();
	cout << S;
	return 0;
}