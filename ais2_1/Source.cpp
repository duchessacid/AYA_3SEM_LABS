#include <iostream>
#include <vector>
#include <cstdint>
#include <cmath>
#include <cstring>
#include <algorithm>
#include <string>
#include <sstream>

using namespace std;
static const unsigned int Mers = pow(2, 31) - 1;

class Bitset {
private:
	size_t max = 0;
	uint8_t* data;
	size_t size = 0;
public:
	Bitset() {}

	~Bitset() {
		delete[] data;
		data = nullptr;
	}

	Bitset(size_t size) {
		this->size = size;
		max = size - 1;
		data = new uint8_t[(size % 8 == 0) ? size / 8 : (size / 8) + 1];
		size_t bytes = (size % 8 == 0) ? size / 8 : (size / 8) + 1;
		for (uint8_t i = 0; i < bytes; i++) data[i] = 0;
	}

	size_t maxval() { return max; }

	bool outofindex(size_t x) {
		return (x < 0 || x > this->maxval()) ? true : false;
	}

	bool is_set(size_t x) {
		size_t byte = x / 8;
		size_t offset = x % 8;
		uint8_t k = (data[x / 8] >> (7 - x % 8)) & 1;
		return k == 1;

	}

	void set(size_t x) {
		if (this->outofindex(x)) return;
		size_t byte = x / 8;
		size_t offset = x % 8;
		data[byte] = (data[byte] | 0b10000000 >> offset);

	}

	void print() {
		string digits(size, '0');
		size_t n = this->realSize();
		for (uint8_t i = 0; i < n; i++) {
			uint8_t ndigits = (i < n - 1) ? 8 : (this->maxval() % 8) + 1;
			for (uint8_t j = 0; j < ndigits; j++) {
				if ((data[i] >> 7 - j) & 1) {
					digits[i * 8 + j] = '1';
				}
			}
		}
		cout << digits << endl;
	}

	size_t realSize() { return (size % 8 == 0) ? size / 8 : (size / 8) + 1; }
};

class Bloomfilter {
private:
	vector<long long> primes_n;
	int m_size = 0;
	int K_hashes = 0;
	Bitset bitset;

public:
	Bloomfilter(int N, double P) {
		m_size = floor(-N * log2(P) / log(2) + 0.5);
		K_hashes = floor(-log2(P) + 0.5);
		cout << m_size << " " << K_hashes << endl;
		bitset = Bitset(m_size);
		primes(round(K_hashes * (log(K_hashes)) + K_hashes + 1));
	}

	void add(unsigned long long int data) {
		for (int i = 0; i < K_hashes; i++) {
			unsigned int hash = ((data % Mers) * (i + 1) % Mers + primes_n[i] % Mers) % Mers % m_size;
			if (!bitset.is_set(hash)) bitset.set(hash);
		}
	}

	void print() {
		bitset.print();
	}

	void search(unsigned long long int data) {
		int i;
		for (i = 0; i < K_hashes; i++) {
			unsigned int hash = ((data % Mers) * (i + 1) % Mers + primes_n[i] % Mers ) % Mers % m_size;
			if (!bitset.is_set(hash)) {
				cout << 0 << endl;
				break;
			}
		}
		if (i == K_hashes)
			cout << 1 << endl;
	}

	void primes(int n) {
		vector<int> lp(n + 1, 0);
		for (int i = 2; i <= n; ++i) {
			if (lp[i] == 0) {
				lp[i] = i;
				primes_n.push_back(i);
			}
			for (int j = 0; j < (int)primes_n.size() && primes_n[j] <= lp[i] && i * primes_n[j] <= n; ++j) lp[i * primes_n[j]] = primes_n[j];

		}
	}
	
	

};


int main() {
	string str, command, val, N, P;
	stringstream ss;
	while (getline(cin, str)) {
		if (str.empty()) continue;
		ss << str;
		ss >> command >> N >> P;
		if (command == "set") {
			if ((!(N.empty())) && (!(P.empty()))) {
				if (stoi(N) > 0 && stod(P) < 0.8 && stod(P) > 0) {
					ss.clear();
					str.clear();
					val.clear();
					break;
				}
				else { cout << "error" << endl; }
			}
			else { cout << "error" << endl; }
		}
		else {
			cout << "error" << endl;
		}
		ss.clear();
		str.clear();
		val.clear();
	}
	if (P.empty() || N.empty() || stoi(N) <= 0 || stod(P) <= 0 || stod(P) >= 0.8) return 0;
	Bloomfilter filt(stoi(N), stod(P));
	string flag;
	while (getline(cin, str)) {
		if (str.empty()) continue;
		ss << str;
		ss >> command >> val >> flag;
		if(!flag.empty()) cout << "error" << endl;
		else if (command == "add") {
			if (val.empty()) cout << "error" << endl;
			filt.add(stoull(val));
		}
		else if (command == "search") {
			if (val.empty()) cout << "error" << endl;
			filt.search(stoull(val));
		}
		else if (command == "print") {
			if ((!val.empty()) || (str.find(" ") != string::npos)) cout << "error" << endl;
			else {
				filt.print();
			}
		}
		else {
			cout << "error" << endl;
		}
		command.clear();
		ss.clear();
		str.clear();
		val.clear();
		flag.clear();
	}
	return 0;
}