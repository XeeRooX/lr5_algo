#define VISUAL
#include <iostream>
#include <vector>
#include <fstream>
#include "Windows.h"
#include <string>
using namespace std;


struct Node 
{
	Node* parent = nullptr;
	Node* child = nullptr;
	Node* left = nullptr;
	Node* right = nullptr;
	int key = 0;
	int degree = 0; //Высота
	bool mark = false; //был ли потерян ребёнок 
};
/////
Node* min = nullptr; //Ссылка на минимальный элеменет
int n = 0; // Количество нодов

// Используется в insert
void listAdd(Node*& r, Node* p);
void listdelete(Node* p);
int Dn() { return (log2(n) + 1); } // Возвращает наибольшую высоту
void consolidate();
void heapLink(Node* y, Node* x);
void cut(Node* x, Node* y);
void cascadingCut(Node* y);
Node* search(Node* r, int k);//search is not good in heap public:
void insert(int k);
int extractMin(); //get minimum node and delete it
int minimum() { return ::min->key; }
void decreaseKey(Node* x, int k);
void remove(int k);
bool search(int k) { return (search(::min, k) == nullptr ? false : true); }
bool empty() { return n == 0; }
void show();



void listAdd(Node*& r, Node* p) {
	if (r == nullptr) {
		r = p;
		r->left = r;
		r->right = r;

	}
	else {
		Node* x = r; //To quote
		p->right = x->right;
		p->left = x;
		x->right->left = p;
		x->right = p;

	}

}

void listdelete(Node* p) {
	p->left->right = p->right;
	p->right->left = p->left;

}


void insert(int k) {
	Node* p = new Node;
	p->key = k;
	listAdd(::min, p);
	if (::min->key > k) {
		::min = p;

	}
	++n;

}

void heapLink(Node* y, Node* x) {
	listdelete(y);
	listAdd(x->child, y);
	++x->degree;
	y->mark = false;

}

void consolidate() {
	vector<Node*> a(Dn(), nullptr);
	Node* x, * y, * z;
	int d;
	Node* sentry = new Node;
	listAdd(::min->left, sentry); 
	for (x = ::min; x != sentry; x = z)
	{
		z = x->right; // Сохранение правой ноды от х
		d = x->degree;
		while (a[d] != nullptr)
		{
			y = a[d];
			if (x->key > y->key)
				swap(x, y);
			heapLink(y, x);
			a[d] = nullptr;
			++d;

		}
		a[d] = x;

	}
	::min = nullptr;
	for (int i = 0; i < a.size(); ++i) {
		if (a[i] != nullptr) {
			listAdd(::min, a[i]);
			if (a[i]->key < ::min->key)
				::min = a[i];

		}

	}
	delete sentry;

}

int extractMin() {
	int ret = 0;
	Node* p = ::min;
	if (p) {
		ret = p->key;
		if (p->child) {
			Node* x = p->child;
			Node* y = x->right;
			for (int i = 0; i < p->degree; ++i) {
				listAdd(::min, x);
				x->parent = nullptr;
				x = y;
				y = y->right;

			}

		}
		if (p->right == p) //the child of p is empty, and p is the only one in root list
			::min = nullptr;
		else {
			::min = p->right;
			listdelete(p);
			consolidate();

		}
		delete p;
		--n;

	}
	return ret;

}

void cut(Node* x, Node* y) {
	listdelete(x);
	--y->degree;
	listAdd(::min, x);
	x->parent = nullptr;
	x->mark = false;

}

void cascadingCut(Node* y) {
	Node* z = y->parent;
	if (z) {
		if (y->mark == false)
			y->mark = true;
		else {
			cut(y, z);
			cascadingCut(z);

		}

	}

}

void decreaseKey(Node* x, int k) {
	if (k >= x->key)
		return;
	x->key = k;
	Node* y = x->parent;
	if (y && y->key > x->key) {
		cut(x, y);
		cascadingCut(y);

	}
	if (x->key < ::min->key)
		::min = x;

}

void remove(int k)
{
	Node* p = search(::min, k);
	if (p == nullptr)
		return;
	decreaseKey(p, INT_MIN);
	extractMin();
}


Node* search(Node* r, int k) {
	if (r == nullptr)
		return r;
	Node* x = r, * y;
	do {
		if (x->key == k)
			return x;
		else if (x->key < k) {
			y = search(x->child, k);
			if (y)
				return y;

		}
		x = x->right;

	} while (x != r);

	return nullptr;

}
void show() 
{
	if (::min != NULL)
	{
		Node* temp = ::min;
		cout << temp->key;
		temp = temp->right;
		while (temp != ::min)
		{
			cout << "-->" << temp->key;
			temp = temp->right;
		}
		cout << endl;
	}
	else
		cout << "\nКуча пуста\n";
}



bool iss_empty(std::fstream& pFile)
{
	return pFile.peek() == std::fstream::traits_type::eof();
}
bool isNum(string a)
{
	for (int i = 0; i < a.length(); i++)
	{
		if (!isdigit(a[i]))
		{
			return false;
		}
	}
	return true;
}
void delF()
{
	ofstream aa("test.txt");
	aa.close();
}
int main() {
	setlocale(LC_ALL, "Rus");
//////////
#ifdef VISUAL

	string msg;
	delF();
	while (true)
	{

		Sleep(1);
		fstream f("test.txt");
		if (!iss_empty(f))
		{
			getline(f, msg);
			if (isNum(msg))
			{
				int num = stoi(msg);
				insert(num);
				cout << "\nДобавлен новый элемент " << num << endl;
				f.close();
				delF();

			}
			else
			{
				if (msg == "DELETEMIN")
				{
					f.close();
					delF();
					int minn = ::min->key;
					//mini = delete_min(mini);
					extractMin();

					cout << "\nУдален минимальный элемент " << minn;
				}
				if (msg == "CONSOLESHOW")
				{
					f.close();
					delF();
					cout << "\nВывод кучи" << endl;
					show();
				}
				if (msg == "EXIT")
				{
					f.close();
					delF();
					return 0;
				}
			}
		}
		f.close();
	}
#endif




/////////
	//FibHeap *fh = new FibHeap();
	insert(1);
	insert(2);
	insert(3);
	insert(4);
	insert(5);
	insert(6);
	insert(7);
	insert(8);
	insert(9);
	//insert(0);
	insert(1);
	insert(2);
	insert(3);
	insert(4);
	insert(5);
	insert(6);
	insert(7);
	insert(8);
	//insert(9);
	extractMin();
	show();
	cout << n << endl;
	extractMin();
	show();
	cout << n << endl;
	extractMin();
	show();
	cout << n << endl;
	extractMin();
	show();
	cout << n << endl;
	extractMin();
	show();
	cout << n << endl;
	extractMin();
	show();
	cout << n << endl;
	extractMin();
	show();
	cout << n << endl;
	extractMin();
	show();

	extractMin();
	show();
	cout << n << endl;

	//fh->insert();
	return 0;
}