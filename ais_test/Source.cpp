#include <stdio.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <iostream>
#include <vector>
#include <list>
#include <string>
#include <algorithm>
#include <queue>
#include <stack>
#include <set>
#include <map>
#include <complex>
using namespace std;
typedef long long lld;
typedef unsigned long long llu;



struct node
{
    int64_t key;
    string value;
    node* parent;
    node* left;
    node* right;
};

class SplayTree
{
    node* root = nullptr;
    void zig(node* n)
    {
        node* p = n->parent;
        if (n->parent->left == n)
        {
            node* A = n->left;
            node* B = n->right;
            node* C = p->right;

            n->parent = nullptr;
            n->right = p;
            p->parent = n;
            p->left = B;

            if (B != NULL) B->parent = p;
        }
        else
        {
            node* A = p->left;
            node* B = n->left;
            node* C = n->right;
            n->parent = nullptr;
            n->left = p;
            p->parent = n;
            p->right = B;

            if (B != nullptr) B->parent = p;
        }
    }

    void zig_zig(node* x)
    {
        node* p = x->parent;
        node* g = p->parent;
        if (p->left == x)
        {
            node* left_child = x->left;
            node* right_child = x->right;
            node* bro = p->right;
            node* unc = g->right;

            x->parent = g->parent;
            x->right = p;

            p->parent = x;
            p->left = right_child;
            p->right = g;

            g->parent = p;
            g->left = bro;


            if (x->parent != nullptr)
            {
                if (x->parent->left == g) x->parent->left = x;
                else x->parent->right = x;
            }

            if (right_child != NULL) right_child->parent = p;

            if (bro != NULL) bro->parent = g;
        }
        else
        {
            node* unc = g->left;
            node* bro = p->left;
            node* left_child = x->left;
            node* right_child = x->right;

            x->parent = g->parent;
            x->left = p;

            p->parent = x;
            p->left = g;
            p->right = left_child;

            g->parent = p;
            g->right = bro;

            if (x->parent != nullptr)
            {
                if (x->parent->left == g) x->parent->left = x;
                else x->parent->right = x;
            }
            if (bro != nullptr) bro->parent = g;

            if (left_child != nullptr) left_child ->parent = p;
        }
    }

    void zig_zag(node* x)
    {
        node* p = x->parent;
        node* g = p->parent;
        cout << "I" << endl;
        if (p->right == x)
        {
            cout << "here" << endl;
            node* bro = p->left;
            node* left_child = x->left;
            node* right_child = x->right;
            node* unc = g->right;

            x->parent = g->parent;
            x->left = p;
            x->right = g;

            p->parent = x;
            p->right = left_child;

            g->parent = x;
            g->left = right_child;

            if (x->parent != nullptr)
            {
                if (x->parent->left == g) x->parent->left = x;
                else x->parent->right = x;
            }

            if (left_child != nullptr) left_child->parent = p;

            if (right_child != nullptr) right_child->parent = g;
        }
        else
        {
            cout << "here!!" << endl;
            node* unc = g->left;
            node* left_child = x->left;
            node* right_child = x->right;
            node* bro = p->right;

            x->parent = g->parent;
            x->left = g;
            x->right = p;

            p->parent = x;
            p->left = right_child;

            g->parent = x;
            g->right = left_child;

            if (x->parent != nullptr)
            {
                if (x->parent->left == g) x->parent->left = x;
                else x->parent->right = x;
            }

            if (left_child != nullptr) left_child->parent = g;

            if (right_child != nullptr) right_child->parent = p;

        }
    }

    void splay(node* x)
    {
        while (x->parent != NULL)
        {
            node* p = x->parent;
            node* g = p->parent;
            if (g == nullptr) zig(x);
            else if (g->left == p && p->left == x) zig_zig(x);
            else if (g->right == p && p->right == x) zig_zig(x);
            else zig_zag(x);
        }
        cout << "here!!!!!!" << endl;
        this->root = x;
    }



public:
    SplayTree() = default;
 
    node* find(int64_t key)
    {
        node* ret = NULL;
        node* curr = this->root;
        node* prev = NULL;
        while (curr != NULL)
        {
            prev = curr;
            if (key < curr->key) curr = curr->left;
            else if (key > curr->key) curr = curr->right;
            else
            {
                ret = curr;
                break;
            }
        }
        if (ret != NULL) splay(ret);
        else if (prev != NULL) splay(prev);
        return ret;
    }
    void insert(int64_t x)
    {
        if (root == nullptr)
        {
            root = new node;
            root->key = x;
            root->left = nullptr;
            root->right = nullptr;
            root->value = "0";
            root->parent = nullptr;
            return;
        }
        node* curr = this->root;
        while (curr != nullptr)
        {
            if (x < curr->key)
            {
                if (curr->left == nullptr)
                {
                    node* newNode = new node;
                    newNode->key = x;
                    newNode->left = nullptr;
                    newNode->right = nullptr;
                    newNode->value = "0";
                    curr->left = newNode;
                    newNode->parent = curr;
                    splay(newNode);
                    return;
                }
                else curr = curr->left;
            }
            else if (x > curr->key)
            {
                if (curr->right == nullptr)
                {
                    node* newNode = new node;
                    newNode->key = x;
                    newNode->left = nullptr;
                    newNode->right = nullptr;
                    newNode->value = "0";
                    curr->left = newNode;
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
    void Delete(int);
    void inOrderPrint(bool);
};















node* subtree_max(node* subRoot)
{
    node* curr = subRoot;
    while (curr->right != NULL) curr = curr->right;
    return curr;
}

node* subtree_min(node* subRoot)
{
    node* curr = subRoot;
    while (curr->left != NULL) curr = curr->left;
    return curr;
}

void SplayTree::Delete(int x)
{
    node* del = find(x);
    if (del == NULL)
    {
        return;
    }
    node* L = del->left;
    node* R = del->right;
    if (L == NULL && R == NULL)
    {
        this->root = NULL;
    }
    else if (L == NULL)
    {
        node* M = subtree_min(R);
        splay(M);
    }
    else if (R == NULL)
    {
        node* M = subtree_max(L);
        splay(M);
    }
    else
    {
        node* M = subtree_max(L);
        splay(M);
        M->right = R;
        R->parent = M;
    }
    delete del;
}

void printTree(node* root)
{
    if (!root) return;
    printTree(root->left);
    cout << "key  " << root->key;
    if (root->left) cout << "  l: " << root->left->key;
    if (root->right) cout << "  r: " << root->right->key;
    cout << endl;
    printTree(root->right);
}

void SplayTree::inOrderPrint(bool brackets)
{
    printTree(this->root);
}

int main()
{
    SplayTree* sTree = new SplayTree();
    sTree->inOrderPrint(true);
    printf("\n---------------------------------------------------------------\n");

    sTree->insert(8);
    sTree->inOrderPrint(true);
    printf("\n---------------------------------------------------------------\n");

    sTree->insert(4);
    sTree->inOrderPrint(true);
    printf("\n---------------------------------------------------------------\n");

    sTree->insert(7);
    sTree->inOrderPrint(true);
    printf("\n---------------------------------------------------------------\n");

    sTree->insert(9);
    sTree->inOrderPrint(true);
    printf("\n---------------------------------------------------------------\n");

    sTree->insert(3);
    sTree->inOrderPrint(true);
    printf("\n---------------------------------------------------------------\n");

    sTree->insert(5);
    sTree->inOrderPrint(true);
    printf("\n---------------------------------------------------------------\n");

    sTree->insert(88);
    sTree->inOrderPrint(true);
    printf("\n---------------------------------------------------------------\n");

    sTree->insert(11);
    sTree->inOrderPrint(true);
    printf("\n---------------------------------------------------------------\n");

    sTree->insert(6);
    sTree->inOrderPrint(true);
    printf("\n---------------------------------------------------------------\n");

   

    return 0;
}


