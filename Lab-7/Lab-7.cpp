#include <iostream>
#include <iterator>
#include <stack>
#include <vector>
#include <algorithm>

using namespace std;
struct Node {
	int key;	//���
	Node* left;	//����� � ������ ���������
	Node* right;
	bool next;
	Node(int k) : key(k), next(0) { left = right = nullptr; }
	Node(int k, Node* left, Node* right) : key(k), left(left), right(right), next(false) {};
	~Node() { delete left; delete right; }
	Node(const Node&) = delete;	//����������� ����������� ��� ����
	friend class Tree;
};
using MyStack = stack<pair<Node*, int>>; //��� ����� ��� ������ ������

//�������� ������
//template<class Container = Set> � ��������� �� ��������� �� ���������
struct myiter : public std::iterator<std::forward_iterator_tag, int>
{
	Node* Ptr;   //�������� ���������
	MyStack St;  //���� � ���� �� ����� ������
	myiter(Node* p = nullptr) : Ptr(p) {}
	myiter(Node* p, MyStack&& St) : Ptr(p), St(move(St)) {}
	bool operator == (const myiter& Other) const { return Ptr == Other.Ptr; }
	bool operator != (const myiter& Other) const { return !(*this == Other); }
	myiter& operator++();
	myiter operator++(int) { myiter temp(*this); ++* this; return temp; }
	pointer operator->() { return &Ptr->key; }
	reference operator*() { return Ptr->key; }
};
//�������� ������� (�������������)
template <typename Container, typename Iter = myiter>
class outiter : public std::iterator<std::output_iterator_tag, typename Container::value_type>
{
protected:
	Container& container;    // ��������� ��� ������� ���������
	Iter iter;	                // ������� �������� ��������� ������
public:
	outiter(Container& c, Iter it) : container(c), iter(it) { } // �����������
	const outiter<Container>&                // ������������ � ������� ����� � ���������
		operator = (const typename Container::value_type& value) {
		iter = container.insert(iter, value).first;
		return *this;
	}
	const outiter<Container>& //������������ ����� � ���������
		operator = (const outiter<Container>&) { return *this; }
	outiter<Container>& operator* () { return *this; } // ������������� � ������ ��������
	outiter<Container>& operator++ () { return *this; } // ��������� � ������ ��������
	outiter<Container>& operator++ (int) { return *this; }
};

// ������� ��� �������� ��������� �������
template <typename Container, typename Iter>
inline outiter<Container, Iter> outinserter(Container& c, Iter it)
{
	return outiter<Container, Iter>(c, it);
}

class Tree {
	//static  size_t tags;
	char tag;
	Node* root;
	int h, count; //������ � �������� ������
	char** SCREEN;
	const int N = 16;
	int offset = 40;
public:          //����������� �������� ����������
	using key_type = int;
	using value_type = int;
	using key_compare = less<int>;

	void swap(Tree& rgt) //����� ����������� ���� ���
	{
		std::swap(tag, rgt.tag); std::swap(root, rgt.root);
		std::swap(count, rgt.count);      std::swap(h, rgt.h);
	}

	//����������� ������� ������� (��������)
	//myiter insert(const myiter& where, const int& k);
	myiter insert(const int& k, const myiter& where = myiter(nullptr))
	{
		return insert(where, k).first;
	}
	myiter begin()const;
	myiter end()const { return myiter(nullptr); }
	pair<myiter, bool> insert(myiter where, int k);
	Tree() : tag('A'), root(nullptr), h(0), count(0), SCREEN(nullptr) {}

	Tree(int* A, int n) : tag('A'), root(nullptr), h(0), count(0) {
		for (int i = 0; i < n; i++) this->insert(A[i]);
	}
	Tree(int nMax) : tag('A'), root(nullptr), h(0), count(0) {
		int* A = Generation(nMax);
		int n;
		for (n = 0; A[n] >= 0; n++);
		for (int i = 0; i < n; i++) this->insert(A[i]);
	}
	int size() { return count; }
	~Tree() { delete root; }
	Tree(const Tree& rgt) : Tree() {
		for (auto x = rgt.begin(); x != rgt.end(); ++x) insert(*x);
	}
	Tree(Tree&& rgt) noexcept : Tree() { swap(rgt); }
	Tree& operator = (const Tree& rgt) {
		Tree temp;
		for (auto x : rgt) temp.insert(x);
		swap(temp);
		return *this;
	}
	Tree& operator = (Tree&& rgt) noexcept { swap(rgt); return *this; }
	Tree operator & (const Tree& t) const {
		Tree r;
		set_intersection((*this).begin(), (*this).end(), t.begin(), t.end(), outinserter(r, myiter(nullptr)));
		return r;
	}
	Tree operator ^ (const Tree& t) const {
		Tree r;
		set_symmetric_difference((*this).begin(), (*this).end(), t.begin(), t.end(), outinserter(r, myiter(nullptr)));
		return r;
	}
	void newOutput();
	void OutNode(Node* V, int row, int col, bool gr);
	int* Generation(int nMax);
	bool find(int k) const;
	friend class Container;
};

bool Tree::find(int k) const {
	Node* t = root;
	while (t) {
		if (k == t->key)
			return true;
		else if (k < t->key)
			t = t->left;
		else
			t = t->right;
	}
	return false;
}

int* Tree::Generation(int nMax) {
	long w = 0;
	w = rand() % 65536;
	int n = 0;
	int* A;
	for (int i = 0; i < N; ++i) {
		if (n < nMax) {
			if ((w >> i) & 1) n++;
		}
	}
	A = new int[n];
	for (int i = 0, k = 0; k < n; ++i)
		if ((w >> i) & 1) A[k++] = i;
	return A;
	/*int* A;
	int n = rand() % nMax;
	A = new int[n];
	for (int i = 0; i < n; i++) {
		A[i] = rand() % N;
	}
	return A;*/
}



myiter Tree::begin()const { //����� ������� �������� ���������
	MyStack St;
	Node* p(root);
	if (p) {   //��� �� ����� �����, ��������� ���� � �����
		while (p->left) {
			St.push(make_pair(p, 0));
			p = p->left;
		}
	}
	return myiter(p, move(St)); //������ ��������, ������� ��� ����
}

pair<myiter, bool> Tree::insert(myiter where, int k)
{
	Node* t{ root };
	bool cont{ true }, up{ false };
	stack<pair<Node*, int>> St;
	if (!where.Ptr) { //������� � ������ ������ ��� ���������
	//===== ������������� =====
		if (t == nullptr) {	// ������ �����
			root = new Node(k);
			count = h = 1;
			St.push(make_pair(root, 1));	// ������������� ������.
			return make_pair(myiter(root, move(St)), true);// � �����
		}
		else St.push(make_pair(root, 1));
		// ������������� �����: ������; ������ 1
	}
	else {  //������ � ����� ���������� �������
		t = where.Ptr;
		St = move(where.St);
	}
	Node* tr = nullptr;
	while (cont) {//����� �� ������
		if (k == t->key) {    // ����� �������� �� �������������
			return make_pair(myiter(t, move(St)), false);
		}
		if (k < t->key) {
			if (t->left) { //��� �����
				St.push(make_pair(t, 2)); //�������� ���������; ������ 2
				t = t->left;
			}
			else if (!t->next) { //������� ������
				t->left = new Node(k);
				cont = false;
				tr = t->left;
			}
			else {
				Node* t1 = new Node(k, nullptr, t);
				cont = false;
				up = true;
				++h;
				t = t1;
			}
		}
		else if (!t->right) { //������� ������� ������
			t->right = new Node(k);
			t->next = true;     //*************************************
			cont = false;
			tr = t->right;
		}
		else if (t->next) { //������ �� ����
			if (k < t->right->key) { //������ �������
				if (t->right->left) { //���� ���� ����
					St.push(make_pair(t, 3)); // � ������ 3
					t = t->right->left;
				}
				else { //������� ��������
					t->right->left = new Node(k);
					cont = false;
					tr = t->right->left;
				}
			}
			else {
				if (t->right->right) { //���� ���� ����
					St.push(make_pair(t, 4)); // � ������ 4
					t = t->right->right;
				}
				else { //������� ��������
					t->right->right = new Node(k);
					up = t->right->next = true; //true, ����� ��� //*********
					cont = false;
					tr = t->right->right;
					++h;
				}
			}
		}
		else if (t->right) {		//���� ������ ����
			St.push(make_pair(t, 3));   // � ������ 3
			t = t->right;
		}
		else {	//������� �������, ����������� ������
			t->right = new Node(k);
			t->next = true;
			up = false;
			cont = false;
		}
		while (up) {	//������ �� ���: �������� ������� �� ������� �����
			Node* t1{ t->right };
			Node* t2{ t->right->right };
			t->next = t1->next = false;
			t->right = t1->left;
			t1->left = t;
			t1->right = t2;

			if (St.top().second == 1) root = t1;
			else if (St.top().second == 4) root->right->right = t1;
			else if (St.top().second == 3) St.top().first->right = t1;

			if (k == t1->key) tr = t1;
			else if (k == t->key) tr = t;
			else if (t2) if (k == t2->key) tr = t2;
			t = St.top().first;
			switch (St.top().second) {
			case 1:	//����� �� ����� � ������ ��������, �����
				++h; //���� ������
				up = false;
				break;
			case 2:	//������� ������
				t->left = t1->left;
				t1->left = t1->right;
				t1->right = t->right;
				t->right = t1;
				t->next = true;
				std::swap(t1->key, t->key);
				tr = t1;
				up = t1->next = t->next;
				break;
			case 3:
				if (t->next) {	//������� ��������, ����������
					t->right->left = t1->right;
					t1->right = t->right;
					t->right = t1;
					t1->next = true;
				}
				else {	//������� ������� ������, �����
					t->next = true;
					up = t1->next = false;
					St.pop();
				}
				break;
			case 4:
				//�������  � ������������� � ������ �������, ����������
				t->right->next = true;
				t1->next = false;
				t1->right = t2;
			}
			if (up) St.pop();
		}
	}
	++count; //���� ��������
	//++h;
	return make_pair(myiter(tr, move(St)), true);
};


//��������� = ��� �� ������, ���������� �����
myiter& myiter::operator++()
{
	if (!Ptr) { //������ ���������?
		return *this; //�� �������� ��� ��������������� ��������� �� ������
	}
	if (Ptr->right) { //��� ������
		St.push(make_pair(Ptr, 1));
		Ptr = Ptr->right;
		while (Ptr->left) { //... � ������ ������� �����
			St.push(make_pair(Ptr, 0));
			Ptr = Ptr->left;
		}
	}
	else { //������ �����, ���� ����� �����
		pair<Node*, int> pp(Ptr, 1);
		while (!St.empty() && pp.second) { pp = St.top(); St.pop(); }
		if (pp.second) //��� ������ � �����, ���������!
		{
			Ptr = nullptr;
		}
		else Ptr = pp.first; //��� ������ � ����������
	}
	return (*this);
}

void Tree::OutNode(Node* V, int row, int col, bool gr = false) {
	if (V) {
		if (col < 80 && col > 0) SCREEN[row - 1][col - 1] = V->key < 10 ? V->key + 48 : V->key + 55;
		if (gr) {
			if (V->left) OutNode(V->left, row + 1, col - 1);
			if (V->right) OutNode(V->right, row + 1, col + (offset >> row) - row - 1);
		}
		else if (V->next) {
			if (V->left) OutNode(V->left, row + 1, col - (offset >> row) + row + 1);
			OutNode(V->right, row, col + 2, true);
		}
		else if (V->left && !V->right) OutNode(V->left, row, col - 2);
		else if (row < h) {
			if (V->left) OutNode(V->left, row + 1, col - (offset >> row) + row + 1);
			if (V->right) OutNode(V->right, row + 1, col + (offset >> row) - row - 1);
		}

	}
}

void Tree::newOutput() {
	if (root) {
		SCREEN = new char* [h];
		for (int i = 0; i < h; ++i) SCREEN[i] = new char[80];
		for (int i = 0; i < h; ++i) {
			memset(SCREEN[i], '.', 80);
			SCREEN[i][79] = 0;
		}

		OutNode(root, 1, offset);
		for (int i = 0; i < h; ++i) {
			std::cout << SCREEN[i] << std::endl;
		}
	}
	else std::cout << "Tree is empty!" << std::endl;
}

class Container {
private:
	Tree set;
	std::vector<myiter> sequence;
public:
	Container() {};
	Container(const Tree&& t) : set(t) {
		for (auto x = set.begin(); x != set.end(); ++x)
			sequence.push_back(x);
	};

	Container operator & (const Container& a) {
		Container r;
		for (auto x : a.sequence) if (set.find(*x))
			r.sequence.push_back(r.set.insert(*x));
		return r;
	}
	Container operator ^ (const Container& a) {
		Container r;
		for (auto x : a.sequence) if (!set.find(*x))
			r.sequence.push_back(r.set.insert(*x));
		for (auto x : sequence) if (!a.set.find(*x))
			r.sequence.push_back(r.set.insert(*x));
		return r;
	}

	void Output() {
		std::cout << "Set:\n";
		set.newOutput();
		std::cout << "Sequence:\n";
		for (auto x : sequence)
			std::cout << *x << " ";
		std::cout << "\n";
	}
	void Mul(int n);
	void Excl(const Container& a);
	void Subst(const Container& a, int p);
	void Concat(const Container& a);

};

void Container::Mul(int n) {
	if (n > 1) {
		auto p = sequence.begin(), q = sequence.end();
		std::vector<myiter> res;
		for (int i = 0; i < n; i++)
			std::copy(p, q, back_inserter(res));
		std::swap(res, sequence);
	}
}

void Container::Excl(const Container& a) {
	auto p = -1, q = -1;
	bool flag = false;
	size_t i_i = 0;
	for (auto i = sequence.begin(); i != sequence.end() and !flag; i++) {
		auto t = a.sequence[0].Ptr->key;
		if (i._Ptr->Ptr->key == a.sequence[0].Ptr->key) {
			auto k = i;
			auto j = a.sequence.begin();
			while (j != a.sequence.end() and k != sequence.end()
				and j._Ptr->Ptr->key == k._Ptr->Ptr->key) {
				k++;
				j++;
			}
			if (j == a.sequence.end()) {
				flag = true;
				p = i_i;
				q = i_i + a.sequence.size();
			}
		}
		i_i++;
	}
	if (flag) {
		Tree temp;
		std::vector<myiter> stemp;
		for (size_t i = 0; i < p; ++i)
			stemp.push_back(temp.insert(*sequence[i]));
		for (size_t i = q; i < sequence.size(); ++i)
			stemp.push_back(temp.insert(*sequence[i]));
		std::swap(temp, set);
		std::swap(stemp, sequence);
	}
}

void Container::Concat(const Container& a) { //���������
	for (auto x : a.sequence) sequence.push_back(set.insert(*x));
}

void Container::Subst(const Container& a, int p)
{
	if (p >= sequence.size()) Concat(a);
	else {
		std::vector<myiter> stemp(sequence.begin(), sequence.begin() + p); //������
		std::copy(a.sequence.begin(), a.sequence.end(), back_inserter(stemp));
		//�������
		std::copy(sequence.begin() + p, sequence.end(), back_inserter(stemp));
		sequence.clear();
		Tree temp;
		for (auto x : stemp)
			sequence.push_back(temp.insert(*x));
		set.swap(temp);
	}
}

int main()
{
	srand(time(nullptr));
	Container C1(Tree(8));
	C1.Output();
	Container C2(Tree(8));
	C2.Output();

	C2.Subst(C1, 0);
	std::cout << "Subst:\n";
	C2.Output();

	C2.Excl(C1);
	std::cout << "Excl:\n";
	C2.Output();

	C2.Mul(3);
	std::cout << "Mul:\n";
	C2.Output();
	std::cout << "\n";

	Container A(Tree(8));
	Container B(Tree(8));
	Container C(Tree(8));
	Container D(Tree(8));
	Container E(Tree(8));
	std::cout << "A:\n";
	A.Output();
	std::cout << "B:\n";
	B.Output();
	std::cout << "C:\n";
	C.Output();
	std::cout << "D:\n";
	D.Output();
	std::cout << "E:\n";
	E.Output();

	Container R = (A & B) ^ (C & D & E);
	std::cout << "A & B ^ C & D & E:\n";
	R.Output();

	return 0;
}
