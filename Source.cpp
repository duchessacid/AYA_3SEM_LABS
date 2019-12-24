#include <iostream>
#include <string>
#include <any>
#include <map>
#include <vector>
#include <fstream>
#include <sstream>
#include "json.h"

using namespace std;

int main() {
	string js = "{\"lastname\" : \"Ivanov\", \"firstname\" : \"Ivan Ivanovich\", \"age\" : 25, \"islegal\" : false, \"marks\" : [4, 5, 5, 5, 2, 3] , \"address\" : { \"city\" : \"Moscow\", \"street\" : \"Vozdvijenka\" } }";
	json object = json::parse(js);
	json object1 = json::parsefile("example_1.json");
	cout << any_cast<int>(object1["size"]) << endl;
	cout << any_cast<string>(object["firstname"]);
	system("pause");
	return 0;
}
