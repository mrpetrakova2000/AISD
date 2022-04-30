#include <iostream>
#include <iterator>
#include <stack>
#include <vector>
#include <algorithm>

using namespace std;
struct Node {
	int key;	//Вес
	Node* left;	//Левое и правое поддерево
	Node* right;
	bool next;
	Node(int k) : key(k), next(0) { left = right = nullptr; }
	Node(int k, Node* left, Node* right) : key(k), left(left), right(right), next(false) {};
	~Node() { delete right; delete left; }
	Node(const Node&) = delete;	//Конструктор копирования для узла
	friend class Tree;
};
using MyStack = stack<pair<Node*, int>>; //Тип стека для обхода дерева

//ИТЕРАТОР ЧТЕНИЯ
//template<class Container = Set> — настройка на контейнер не требуется
struct myiter : public std::iterator<std::forward_iterator_tag, int>
{
	Node* Ptr;   //Реальный указатель
	MyStack St;  //Стек с путём от корня дерева
	myiter(Node* p = nullptr) : Ptr(p) {}
	myiter(Node* p, MyStack&& St) : Ptr(p), St(move(St)) {}
	bool operator == (const myiter& Other) const { return Ptr == Other.Ptr; }
	bool operator != (const myiter& Other) const { return !(*this == Other); }
	myiter& operator++();
	myiter operator++(int) { myiter temp(*this); ++* this; return temp; }
	pointer operator->() { return &Ptr->key; }
	reference operator*() { return Ptr->key; }
};
//ИТЕРАТОР ВСТАВКИ (универсальный)
template <typename Container, typename Iter = myiter>
class outiter : public std::iterator<std::output_iterator_tag, typename Container::value_type>
{
protected:
	Container& container;    // Контейнер для вставки элементов
	Iter iter;	                // Текущее значение итератора чтения
public:
	outiter(Container& c, Iter it) : container(c), iter(it) { } // Конструктор
	const outiter<Container>&                // Присваивание — вставка ключа в контейнер
		operator = (const typename Container::value_type& value) {
		iter = container.insert(iter, value).first;
		return *this;
	}
	const outiter<Container>& //Присваивание копии — фиктивное
		operator = (const outiter<Container>&) { return *this; }
	outiter<Container>& operator* () { return *this; } // Разыменование — пустая операция
	outiter<Container>& operator++ () { return *this; } // Инкремент — пустая операция
	outiter<Container>& operator++ (int) { return *this; }
};

// Функция для создания итератора вставки
template <typename Container, typename Iter>
inline outiter<Container, Iter> outinserter(Container& c, Iter it)
{
	return outiter<Container, Iter>(c, it);
}

class Tree {
	//static  size_t tags;
	char tag;
	Node* root;
	int h, count; //Высота и мощность дерева
	char** SCREEN;
	const int N = 16;
	int offset = 40;
public:          //Стандартные элементы контейнера
	using key_type = int;
	using value_type = int;
	using key_compare = less<int>;

	void swap(Tree& rgt) //Обмен содержимого двух ДДП
	{
		std::swap(tag, rgt.tag); std::swap(root, rgt.root);
		std::swap(count, rgt.count);      std::swap(h, rgt.h);
	}

	//Стандартная функция вставки (оболочка)
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
		return std::move(r);
	}
	Tree operator ^ (const Tree& t) const {
		Tree r;
		set_symmetric_difference((*this).begin(), (*this).end(), t.begin(), t.end(), outinserter(r, myiter(nullptr)));
		return std::move(r);
	}
	void newOutput();
	void OutNode(Node* V, int row, int col, bool gr);
	int* Generation(int nMax);
	friend class Container;
};

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



myiter Tree::begin()const { //Поиск первого элемента множества
	MyStack St;
	Node* p(root);
	if (p) {   //Идём по левой ветви, запоминая путь в стеке
		while (p->left) {
			St.push(make_pair(p, 0));
			p = p->left;
		}
	}
	return myiter(p, move(St)); //Создаём итератор, передаём ему стек
}

pair<myiter, bool> Tree::insert(myiter where, int k)
{
	Node* t{ root };
	bool cont{ true }, up{ false };
	stack<pair<Node*, int>> St;
	if (!where.Ptr) { //Вставка в пустое дерево или свободная
	//===== Инициализация =====
		if (t == nullptr) {	// Дерево пусто
			root = new Node(k);
			count = h = 1;
			St.push(make_pair(root, 1));	// Инициализация стека… 
			return make_pair(myiter(root, move(St)), true);// и выход
		}
		else St.push(make_pair(root, 1));
		// Инициализация стека: корень; случай 1
	}
	else {  //Начать с места предыдущей вставки
		t = where.Ptr;
		St = move(where.St);
	}
	while (cont) {//Поиск по дереву
		if (k == t->key)    // Выход «вставка не понадобилась»
			return make_pair(myiter(t, move(St)), false);
		if (k < t->key) {
			if (t->left) { //Идём влево
				St.push(make_pair(t, 2)); //опускаем указатель; случай 2
				t = t->left;
			}
			else { //Вставка левого
				//t->left = new Node(k, nullptr, t->left);
				//cont = false;
				/*NEWCODE*/
				if (!t->right) {
					t->right = new Node(k, nullptr, nullptr);
					t->next = true;
					std::swap(t->key, t->right->key);
					cont = false;
				}
				else if (t->next) {
					t->right->right = new Node(t->right->key, nullptr, nullptr);
					t->right->key = t->key;
					t->key = k;
					t->right->next = up = true;
					cont = false;
				}
				/*********/
			}
		}
		else if (!t->right) { //Вставка второго справа
			t->right = new Node(k);
			t->next = true;     //*************************************
			cont = false;
		}
		else if (t->next) { //Группа из двух
			if (k < t->right->key) { //Меньше правого
				if (t->right->left) { //Есть путь вниз
					St.push(make_pair(t, 3)); // — случай 3
					t = t->right->left;
				}
				else { //Вставка среднего
					//t->right->left = new Node(k, nullptr, t->right->left);
					//cont = false;
					/*NEWCODE*/
					t->right->right = new Node(t->right->key, nullptr, nullptr);
					t->right->key = k;
					t->right->next = up = true;
					cont = false;
					/*********/
				}
			}
			else {
				if (t->right->right) { //Есть путь вниз
					St.push(make_pair(t, 4)); // — случай 4
					t = t->right->right;
				}
				else { //Вставка третьего
					t->right->right = new Node(k);
					/*My new code*/
					//++h;
					/**/
					up = t->right->next = true; //true, стало три //*********
					cont = false;
				}
			}
		}
		else if (t->right) {		//Есть правый ниже
			St.push(make_pair(t, 3));   // — случай 3
			t = t->right;
		}
		else {	//Вставка второго, образование группы
			t->right = new Node(k);
			t->next = true;     //************************************ 
			cont = false;
		}
		while (up) {	//Группа из трёх: передача второго на уровень вверх
			std::swap(t->key, t->right->key);
			Node* t1{ t->right };
			t->next = t1->next = false;
			t->right = t->right->right;
			t1->right = t1->left;
			t1->left = t->left;
			t->left = t1;
			t1 = t;
			t = St.top().first;
			switch (St.top().second) {
			case 1:	//Дошли до корня — дерево подросло, конец
				++h; //Счёт высоты
				up = false;
				break;
			case 2:	//Вставка левого
				/*My new code*/
				if (t->next) {
					t1->next = true;
				}
				else {
					up = false;
					St.pop();
				}
				t->left = t1->left;
				t1->left = t1->right;
				t1->right = t->right;
				t->right = t1;
				t->next = true;
				std::swap(t1->key, t->key);
				break;
			case 3:
				if (t->next) {	//Вставка среднего, продолжать
					t->right->left = t1->right;
					t1->right = t->right;
					t->right = t1;
					t1->next = true;
				}
				else {	//Вставка второго справа, конец
					t->next = true;
					up = t1->next = false;
					St.pop();
				}
				break;
			case 4:
				//Вставка  — присоединение к группе третьим, продолжать
				t->right->next = true;
				t1->next = false;
			}
			if (up) St.pop();
		}
	}
	++count; //Счёт мощности
	return make_pair(myiter(t, move(St)), true);
};


//Инкремент = шаг по дереву, внутренний обход
myiter& myiter::operator++()
{
	if (!Ptr) { //Первое обращение?
		return *this; //Не работает без предварительной установки на дерево
	}
	if (Ptr->right) { //Шаг вправо
		St.push(make_pair(Ptr, 1));
		Ptr = Ptr->right;
		while (Ptr->left) { //... и искать крайний левый
			St.push(make_pair(Ptr, 0));
			Ptr = Ptr->left;
		}
	}
	else { //Подъём вверх, пока слева пусто
		pair<Node*, int> pp(Ptr, 1);
		while (!St.empty() && pp.second) { pp = St.top(); St.pop(); }
		if (pp.second) //Шаг вправо — тупик, завершить!
		{
			Ptr = nullptr;
		}
		else Ptr = pp.first; //Шаг вправо и продолжать
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
	Container(const Tree& t) : set(t) {
		for (auto x = set.begin(); x != set.end(); ++x)
			sequence.push_back(x);

	};

	Container operator & (const Container& a) const {
		Tree t = set & a.set;
		Container r(t);
		return r;
	}
	Container operator ^ (const Container& a) const {
		Tree t = set ^ a.set;
		Container r(t);
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
	int i_i = 0;
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
			temp.insert(*sequence[i]);
		for (size_t i = q; i < sequence.size(); ++i)
			temp.insert(*sequence[i]);
		for (auto x = temp.begin(); x != temp.end(); x++)
			stemp.push_back(x);
		std::swap(temp, set);
		std::swap(stemp, sequence);
	}
}

void Container::Concat(const Container& a) { //Сцепление
	for (auto x : a.sequence) set.insert(*x);
	std::copy(a.sequence.begin(), a.sequence.end(), back_inserter(sequence));
}

void Container::Subst(const Container& a, int p)
{
	if (p >= sequence.size()) Concat(a);
	else {
		std::vector<myiter> stemp(sequence.begin(), sequence.begin() + p); //Начало
		std::copy(a.sequence.begin(), a.sequence.end(), back_inserter(stemp));
		//Вставка
		std::copy(sequence.begin() + p, sequence.end(), back_inserter(stemp));
		sequence.clear();
		for (auto x : stemp)
			sequence.push_back(x);
		for (auto x : a.sequence)
			set.insert(*x);
	}
}

int main()
{
	//srand(time(nullptr));
	/*
	int a[] = {10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0};
	Tree t(a , 11);
	//t.newOutput();
	Container c(t);
	c.Output();
	std::cout << "\n";

	int b[] = {4, 5, 1, 7, 9, 11, 2, 3, 10};
	Tree t1(b, 9);
	Container c1(t1);
	c1.Output();
	std::cout << "\n";

	Container r = c & c1;
	r.Output();
	std::cout << "\n";*/
	/*
	Container c(r);
	c.Mul(2);
	c.Output();*/


	//int a[] = { 1,2,3,4,5,6,7,8 };
	//Tree t(a, 8);
	//Tree t;
	//Container c(t);
	//c.Output();

	/*int b[] = {6, 5, 4};
	Tree t1(b, 3);
	Container c1(t1);
	c1.Output();

	auto r = c1 ^ c;
	//r.Output();

	c.Subst(c1, 2);
	//c.Output();
	c.Excl(c1);
	//c.Output();
	*/
	//Tree T(5);
	//int a[] = { 9, 3, 14, 4, 1, 12, 6, 14, 2, 0, 9, 1, 1, 11, 9 };
	//Tree T(a, 15);
	Container C2(Tree(5));
	C2.Output();
	Container C1(Tree(5));
	C1.Output();
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

	Container A(Tree(5));
	Container B(Tree(5));
	Container C(Tree(5));
	Container D(Tree(5));
	Container E(Tree(5));
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
