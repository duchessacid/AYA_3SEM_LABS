#include "SharedPtr.h"
#include "counter.h"
#include <iostream>
using namespace std;

int main() {
	int* a = new int(5);
	SharedPtr<int> P1(a);
	SharedPtr<int> P2(P1);
	SharedPtr<int> P3(P2);
	SharedPtr<int> P4(P3);
	P3.reset();
	P4.reset();
	{
		SharedPtr<int> P5(P1);
		cout << P1.use_count() << endl; 
		cout << P5.use_count() << endl;

	}
	cout << P2.use_count();
	system("pause");
	return 0;




}
