
#include <iostream>
#include <string>
#include <sstream>
#include<queue>
using namespace std;

class SplayTree
{
    struct node
    {
        int64_t key = 0;
        string value;
        node* parent;
        node* left;
        node* right;
        node(int64_t key, const string& value) {
            this->key = key;
            this->value = move(value);
            this->parent = nullptr;
            this->left = nullptr;
            this->right = nullptr;
        }
    };

    node* root = nullptr;

    void zag(node* x) {
        node* l = x->left;
        node* p = x->parent;
        node* pp = p->parent;
        p->right = l;
        if (l) l->parent = p;
        x->parent = pp;
        if (pp) (p == pp->left ? pp->left : pp->right) = x;
        p->parent = x;
        x->left = p;
    }

    void zig(node* x) {
        node* r = x->right;
        node* p = x->parent;
        node* pp = p->parent;
        p->left = r;
        if (r) r->parent = p;
        x->parent = pp;
        if (pp) (p == pp->left ? pp->left : pp->right) = x;
        p->parent = x;
        x->right = p;
    }

    void zig_zig(node* x) {
        zig(x->parent);
        zig(x);
    }

    void zag_zag(node* x) {
        zag(x->parent);
        zag(x);
    }

    void zig_zag(node* x) {
        zig(x);
        zag(x);
    }

    void zag_zig(node* x) {
        zag(x);
        zig(x);
    }

    void splay(node* x) {
        node* p = x->parent;
        node* pp = nullptr;
        while (p) {
            pp = p->parent;
            if (pp == nullptr) p->left == x ? zig(x) : zag(x);
            else if (x == p->left)  p == pp->left ? zig_zig(x) : zig_zag(x);
            else p == pp->right ? zag_zag(x) : zag_zig(x);
            p = x->parent;
        }
        this->root = x;
    }

    
    void clear(node*& root) {
        if (root == nullptr) return;
        clear(root->left);
        clear(root->right);
        delete root;
        root = nullptr;
    }

    bool find(int64_t x)
    {
        if (!root) {
            return 0;
        }
        node* curr = this->root;
        node* prev = curr;
        while (curr != nullptr)
        {
            prev = curr;
            if (x < curr->key) curr = curr->left;
            else if (x > curr->key) curr = curr->right;
            else break;
        }
        if (curr == nullptr) {
            splay(prev);
            return 0;
        }
        splay(curr);
        return root->key == x;
    }

    node* search_del(int64_t x) {
        if (!root) {
            return root;
        }
        node* curr = this->root;
        node* prev = nullptr;
        node* prev1 = nullptr;
        while (curr != nullptr)
        {
            prev = curr;
            if (x < curr->key) curr = curr->left;
            else if (x > curr->key) curr = curr->right;
            else {
                prev1 = curr;
                break;
            }
        }
        if (curr == nullptr) {
            splay(prev);
            return nullptr;
        }
        splay(prev1);
        return prev1;
    }

    node* find_end(node* root)
    {
        node* curr = root;
        while (curr->right != nullptr) curr = curr->right;
        return curr;
    }

    node* delet1(int64_t x)
    {
        node *temp = search_del(x);
        if (temp == nullptr) {
            cout << "error" << endl;
            return root;
        }
        node* l = temp->left;
        node* r = temp->right;
        if (l) l->parent = nullptr;
        if (r) r->parent = nullptr;
        delete temp;
        temp = nullptr;
        if (!l && !r ) this->root = nullptr;
        else if (!l) splay(r);
        else if (!r) splay(l);
        else
        {
            l = find_end(l);
            splay(l);
            l->right = r;
            r->parent = l;
        }
        return root;
    }

   
public:
    SplayTree() = default;
    ~SplayTree() { clear(root); }
    void add(int64_t key, const string& value)
    {
        if (root == nullptr)
        {
            root = new node(key, value);
            return;
        }
        node* curr = this->root;
        while (curr != nullptr)
        {
            if (key < curr->key)
            {
                if (curr->left == nullptr)
                {
                    node* newNode = new node(key, value);
                    curr->left = newNode;
                    newNode->parent = curr;
                    splay(newNode);
                    return;
                }
                else curr = curr->left;
            }
            else if (key > curr->key)
            {
                if (curr->right == nullptr)
                {
                    node* newNode = new node(key, value);
                    curr->right = newNode;
                    newNode->parent = curr;
                    splay(newNode);
                    return;
                }
                else curr = curr->right;
            }
            else
            {
                cout << "error" << endl;
                splay(curr);
                return;
            }
        }
    }
    
    string search(const int64_t key) {
        ostringstream s;
        if (find(key)) s << 1 << " " << root->value;
        else {
            s << 0;
        }
        return s.str();
    }

    void set(int64_t key, const string& value) {
        if (!find(key))
        {
            cout << "error" << endl;
            return;
        }
        root->value = move(value);
    }

    void delet(int64_t key) {
        root = delet1(key);
    }

    string max1() {
        ostringstream s;
        if (root == nullptr) {
            s << "error";
            return s.str();
        }
        node* temp = root;
        while (temp->right != nullptr) {
            temp = temp->right;
        }
        splay(temp);
        s << temp->key << " " << temp->value;
        return s.str();
    }

    string min() {
        ostringstream s;
        if (root == nullptr) {
            s << "error";
            return s.str();
        }
        node* temp = root;
        while (temp->left != nullptr) {
            temp = temp->left;
        }
        splay(temp);
        s << temp->key << " " << temp->value;
        return s.str();
    }

    void print1(node* root) {
        if (root == nullptr) return;
        queue<node*> que;
        node* n;
        que.push(root);
        bool allChildNull = false;
        while (que.size() > 0 && !allChildNull) {
            allChildNull = true;
            queue<node*> childQue;
            while (!que.empty()) {
                n = que.front();
                que.pop();
                if (n == nullptr && que.empty()) cout << "_";
                else if (n == nullptr) cout << "_ ";
                else if (n->parent != nullptr && que.empty()) cout << "[" << n->key << " " << n->value << " " << n->parent->key << "]";
                else if (n->parent != nullptr) cout << "[" << n->key << " " << n->value << " " << n->parent->key << "] ";
                else cout << "[" << n->key << " " << n->value << "] ";
                if (n == nullptr) {
                    childQue.push(nullptr);
                    childQue.push(nullptr);
                }
                else {
                    childQue.push(n->left);
                    childQue.push(n->right);
                    if (n->left != nullptr || n->right != nullptr) allChildNull = false;
                }
            }
            cout << endl;
            swap(que, childQue);
        }
    }

    void print() {
        if (!root) {
            cout << "_" << endl;
            return;
        }
        print1(root);
    }
};


int main()
{
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
            else tree.add(stoull(val1), val2);
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
