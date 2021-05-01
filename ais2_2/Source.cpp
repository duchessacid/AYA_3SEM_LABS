#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
using namespace std;

struct node {
	int64_t key = 0;
	string value;
	node* left; 
	node* right;
	node* parent; 
};

class SplayTree {
	node* r = nullptr;

	void rotleft(node*& n) {
		if (n->parent)
		{
			if (n->parent->right == n) n->parent->right = n->right;
			else n->parent->left = n->right;
		}
		if (n->right->left) n->right->left->parent = n;
		n->right->parent = n->parent;
		n->right->left = n;
		n->parent = n->right;
		n->right = n->right->left;
	}

	void rotright(node*& n) {
		if (n->parent)
		{
			if (n->parent->right == n) n->parent->right = n->left;
			else n->parent->left = n->left;
		}
		if (n->left->right) n->left->right->parent = n;
		n->left->parent = n->parent;
		n->left->right = n;
		n->parent = n->left;
		n->left = n->left->right;
	}

	node* splay(node* n) {
		while (n->parent != NULL)
		{
			if (!n->parent) break;
			if (!n->parent->parent)
			{
				if (n->parent->left == n) rotright(n->parent);
				else rotleft(n->parent);
				break;
			}
			if (n->parent->parent->left == n->parent)
			{
				if (n->parent->left == n){
					rotright(n->parent->parent);
					rotright(n->parent);
				}
				else{
					rotleft(n->parent);
					rotright(n->parent->parent);
				}
			}
			else
			{
				if (n->parent->left == n){
					rotright(n->parent);
					rotleft(n->parent->parent);
				}
				else{
					rotleft(n->parent->parent);
					rotleft(n->parent);
				}
			}
		}
		this->r = n;
		return r;
	}

	node* add(node*& root, const int64_t key, const string& value) {
		if (root == nullptr) {
			root = new node;
			root->key = key;
			root->value = move(value);
			root->left = nullptr;
			root->right = nullptr;
			root->parent = nullptr;
			return root;
		}
		node* newnode = new node;
		newnode->key = key;
		newnode->value = move(value);
		while (root != nullptr) {
			if (key < root->key) {
				if (root->left == nullptr) {
					root->left = newnode;
					newnode->parent = root;
					newnode->left = nullptr;
					newnode->right = nullptr;
					return splay(newnode);
				}
				else root = root->left;
			}
			else {
				if (root->right == nullptr) {
					newnode->left = nullptr;
					newnode->right = nullptr;
					newnode->parent = root;
					root->right = newnode;
					return splay(newnode);
				}
				else root = root->right;
			}
		}
	}

	node* set(node* root, const int64_t key, const string& val) {
		if (root == nullptr) {
			cout << "error" << endl;
			return nullptr;
		}
		root->value = move(val);
		return root;
	}

	bool search(node*& root, const int64_t key) {
		while (root)
		{
			if (root->key == key) break;
			if (key < (root->key)){
				if (root->left) root = root->left;
				else break;
			}
			else {
				if (root->right) root = root->right;
				else break;
			}
		}
		root = splay(root);
		return root->key == key;
	}

	node* delet(node* root, int64_t key) {
		root = splay(root);
		node* temp = root;
		if (root->left == nullptr) {
			temp = root;
			root = root->right;
			root->parent = nullptr;
			delete temp;
			return root;
		}
		while (root->left->right) root->left = root->left->right;
		if (root->right)
		{
			root->left->right = temp->right;
			temp->right->parent = root->left;
		}
		root = temp->left;
		root->parent = nullptr;
		delete temp;
		return root;
	}

	void clear(node*& root) {
		if (root == nullptr) return;
		clear(root->left);
		clear(root->right); 
		delete root; 
		root = nullptr;
	}

	void InOrder(node* root)
	{
		if (root == nullptr) return;
		InOrder(root->left);
		if (root) cout << "key  " << root->key;
		if (root->left) cout << "  left  " << root->left->key;
		if (root->right) cout << "  right " << root->right->key;
		cout << endl;
		InOrder(root->right);

	}

	int height(node* p)
	{
		int h1 = 0, h2 = 0;
		if (p == nullptr)return(0);
		if (p->left) { h1 = height(p->left); }
		if (p->right) { h2 = height(p->right); }
		return(max(h1, h2) + 1);
	}

public:
	SplayTree() = default;

	~SplayTree() { clear(r); }

	void add(int64_t key, const string& value) {
		r = add(r, key, value);
	}

	void set(int64_t key, const string& value) {
		if (!search(r, key)) {
			cout << "error" << endl;
			return;
		}
		r = set(r, key, value);
	}

	void delet(int64_t key) {
		if (!search(r, key)) {
			cout << "error" << endl;
			return;
		}
		r = delet(r, key);
	}

	string search(const int64_t key) {
		ostringstream s;
		if (!r) s << "error";
		else {
			if (search(r, key)) s << 1 << " " << r->value;
			else {
				s << 0;
			}
		}
		return s.str();
	}
		
	string max1() {
		ostringstream s;
		if (r == nullptr) {
			s << "error";
			return s.str();
		}
		node* temp = r;
		while (temp->right != nullptr) {
			temp = temp->right;
		}
		r = splay(temp);
		s << temp->key << "  " << temp->value;
		delete temp;
		return s.str();
	}

	string min() {
		ostringstream s;
		if (r == nullptr) {
			s << "error";
			return s.str();
		}
		node* temp = r;
		while (temp->left != nullptr) {
			temp = temp->left;
		}
		r = splay(temp);
		s << temp->key << "  " << temp->value;
		delete temp;
		return s.str();
	}

	void print1(node* root, int level) {
		if (root != nullptr) {
			level++;
			print1(root->right, level);

			for (int i = 0; i < level; i++) {
				cout << "  ";
			}
			cout << root->key << endl;

			print1(root->left, level);
		}
	}

	void print() {
		int64_t parent = 0;
		if (r == nullptr) {
			cout << "_" << endl;
			return;
		}
		InOrder(r);
		//cout << "[" << r->key << " " << r->value << "]" << endl;
		//for (int i = 1; i < height(r); i++) {
			//(r, i, parent);
			//cout << endl;
		//}
	}
	
};


int main() {
	string str, command, val1, val2;
	stringstream ss;
	SplayTree tree;
	string flag;
	while (getline(cin, str)) {
		if (str.empty()) continue;
		ss << str;
		ss >> command >> val1 >> val2 >> flag;
		if (!flag.empty()) cout << "error" << endl;
		else if (command == "add") {
			if (val1.empty() || val2.empty()) cout << "error" << endl;
			tree.add(stoull(val1), val2);
		}
		else if (command == "set") {
			if (val1.empty() || val2.empty()) cout << "error" << endl;
			tree.set(stoull(val1), val2);
		}
		else if (command == "search") {
			if (val1.empty()) cout << "error" << endl;
			cout << tree.search(stoull(val1)) << endl;
		}
		else if (command == "delete") {
			if (val1.empty()) cout << "error" << endl;
			tree.delet(stoull(val1));
		}
		else if (command == "print") {
			if ((!val1.empty()) || (str.find(" ") != string::npos)) cout << "error" << endl;
			else {
				tree.print();
			}
		}
		else if (command == "min") {
			if ((!val1.empty()) || (str.find(" ") != string::npos)) cout << "error" << endl;
			cout << tree.min() << endl;
		}
		else if (command == "max") {
			if ((!val1.empty()) || (str.find(" ") != string::npos)) cout << "error" << endl;
			cout << tree.max1() << endl;
		}
		else {
			cout << "error" << endl;
		}
		command.clear();
		ss.clear();
		str.clear();
		val1.clear();
		val2.clear();
		flag.clear();
	}
	return 0;
}









