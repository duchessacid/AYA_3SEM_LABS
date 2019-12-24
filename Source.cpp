#include <iostream>
#include <ctime>
#include <vector>
#include <cstdlib>
#define fst(x) x x x x x x x x x x
#define sec(x) \
 fst(x) fst(x) fst(x) fst(x) fst(x) fst(x) fst(x) fst(x) fst(x) fst(x) 
#define thd(x) \
 sec(x) sec(x) sec(x) sec(x) sec(x) sec(x) sec(x) sec(x) sec(x) sec(x)
#define fot(x) \
 thd(x) thd(x) thd(x) thd(x) thd(x) thd(x) thd(x) thd(x) thd(x) thd(x)
#define fit(x) \
fot(x) fot(x) fot(x) fot(x) fot(x) fot(x) fot(x) fot(x) fot(x) fot(x)
using namespace std;

void direct_test(size_t size) {
	size /= 4;
	vector <int> v(size);
	int i = 0;
	for (; i < size; i += 64) {
		v[i] = i + 64;
	}
	i -= 64;
	v[i] = 0;
	i = 0;
	size_t k = 0;
	unsigned int beg = clock();
	for (k = 0; k < 1000;) {
		fit(i = v[i];) k++;
	}
	unsigned en = clock();
	std::cout << (en - beg) / 100. << " ns\n";
}

void back_test(size_t size) {
	size /= 4;
	vector <int> v1(size);
	int i = size - 1;
	for (; i >= 0; i -= 64) {
		v1[i] = i - 64;
	}
	i += 64;
	v1[i] = size - 1;
	size_t k = 0;
	unsigned int beg = clock();
	for (k = 0; k < 1000;) {
		fit(i = v1[i];) k++;
	}
	unsigned int en = clock();
	std::cout << (en - beg) / 100. << " ns\n";
}

void random_test(size_t size) {
	size /= 4;
	vector <int> v2(size);
	int n = size / 64;
	vector<int> rand_v(n);
	for (int i = 0; i < n; i++) {
		rand_v[i] = i * 64;
	}
	size_t k = 0;
	while (n > 0) {
		srand(time(NULL));
		time_t t = clock();
		srand(t);
		int ind = rand() % n;
		v2[k] = rand_v[ind];
		n--;
		for (size_t j = ind; j < (n); j++) {
			rand_v[j] = rand_v[j + 1];
		}
		rand_v.resize(n);
		k += 64;
	}
	int i = 0;
	unsigned int beg = clock();
	for (k = 0, i = 0; k < 1000;) {
		fit(i = v2[i];) k++;
	}
	unsigned int en = clock();
	std::cout << (en - beg) / 100. << " ns\n";
}




int main() {
	//L1 - 128 Kb; L2 - 512 Kb; L3 - 4096 Kb;
	direct_test(8 * 1024 * 1024);
	int count = 1;
	cout << "investigaion:" << '\n';
	cout << "travel_order: \"direct\"" << '\n';
	cout << "experiments:" << '\n';
	for (size_t k = 65536; k <= 16 * 1024 * 1024; k *= 2) {
		cout << "-experiment:" << '\n';
		cout << "number: " << count << '\n';
		cout << "input_data: " << '\n';
		cout << "buffer_size: " << k / 1024 << " mb" << '\n';
		cout << "results: " << '\n';
		cout << "duration: ";
		direct_test(k);
		cout << '\n';
		count++;
	}
	cout << '\n';
	count = 1;
	back_test(8 * 1024 * 1024);
	cout << "investigaion:" << '\n';
	cout << "travel_order: \"back\"" << '\n';
	cout << "experiments:" << '\n';
	for (size_t k = 65536; k <= 16 * 1024 * 1024; k *= 2) {
		cout << "-experiment:" << '\n';
		cout << "number: " << count << '\n';
		cout << "input_data: " << '\n';
		cout << "buffer_size: " << k / 1024 << " mb" << '\n';
		cout << "results: " << '\n';
		cout << "duration: ";
		back_test(k);
		cout << '\n';
		count++;
	}
	cout << '\n';
	count = 1;
	random_test(8 * 1024 * 1024);
	cout << "investigaion:" << '\n';
	cout << "travel_order: \"Random\"" << '\n';
	cout << "experiments:" << '\n';
	for (size_t k = 65536; k <= 16 * 1024 * 1024; k *= 2) {
		cout << "-experiment:" << '\n';
		cout << "number: " << count << '\n';
		cout << "input_data: " << '\n';
		cout << "buffer_size: " << k / 1024 << " kb" << '\n';
		cout << "results: " << '\n';
		cout << "duration: ";
		random_test(k);
		cout << '\n';
		count++;
	}
	system("pause");
	return 0;
}
