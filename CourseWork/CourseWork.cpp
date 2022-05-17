#include <iostream>
#include <iterator>
#include <stack>
#include <vector>
#include <algorithm>
#include <chrono>
#include <ctime>
#include <fstream>
#include <set>

const int N = 1000;
using namespace std;
struct Node {
	int key;	//Вес
	Node* left;	//Левое и правое поддерево
	Node* right;
	bool next;
	Node(int k) : key(k), next(0) { left = right = nullptr; }
	Node(int k, Node* left, Node* right) : key(k), left(left), right(right), next(false) {};
	~Node() { if (left) delete left; if (right) delete right; }
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
	//const int N = 16;
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
		for (int i = 0; i < n; i++) {
			int k = i;
			this->insert(k);
		}
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
	int* A;
	int n = rand() % nMax;
	A = new int[n];
	for (int i = 0; i < n; i++) {
		A[i] = rand() % N;
		this->insert(i);
	}
	return A;
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
			St.push(make_pair(root, 1));	// Инициализация стека….
			return make_pair(myiter(root, move(St)), true);// и выход
		}
		else St.push(make_pair(root, 1));
		// Инициализация стека: корень; случай 1
	}
	else {  //Начать с места предыдущей вставки
		t = where.Ptr;
		St = move(where.St);
	}
	Node* tr = nullptr;
	while (cont) {//Поиск по дереву
		if (k == t->key) {    // Выход «вставка не понадобилась»
			return make_pair(myiter(t, move(St)), false);
		}
		if (k < t->key) {
			if (t->left) { //Идём влево
				St.push(make_pair(t, 2)); //опускаем указатель; случай 2
				t = t->left;
			}
			else if (!t->next) { //Вставка левого
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
		else if (!t->right) { //Вставка второго справа
			t->right = new Node(k);
			t->next = true;     //*************************************
			cont = false;
			tr = t->right;
		}
		else if (t->next) { //Группа из двух
			if (k < t->right->key) { //Меньше правого
				if (t->right->left) { //Есть путь вниз
					St.push(make_pair(t, 3)); // — случай 3
					t = t->right->left;
				}
				else { //Вставка среднего
					t->right->left = new Node(k);
					cont = false;
					tr = t->right->left;
				}
			}
			else {
				if (t->right->right) { //Есть путь вниз
					St.push(make_pair(t, 4)); // — случай 4
					t = t->right->right;
				}
				else { //Вставка третьего
					t->right->right = new Node(k);
					up = t->right->next = true; //true, стало три //*********
					cont = false;
					tr = t->right->right;
					++h;
				}
			}
		}
		else if (t->right) {		//Есть правый ниже
			St.push(make_pair(t, 3));   // — случай 3
			t = t->right;
		}
		else {	//Вставка второго, образование группы
			t->right = new Node(k);
			t->next = true;
			up = false;
			cont = false;
		}
		while (up) {	//Группа из трёх: передача второго на уровень вверх
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
			case 1:	//Дошли до корня — дерево подросло, конец
				++h; //Счёт высоты
				up = false;
				break;
			case 2:	//Вставка левого
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
				t1->right = t2;
			}
			if (up) St.pop();
		}
	}
	++count; //Счёт мощности
	//++h;
	return make_pair(myiter(tr, move(St)), true);
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

void Container::Concat(const Container& a) { //Сцепление
	for (auto x : a.sequence) sequence.push_back(set.insert(*x));
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
		Tree temp;
		for (auto x : stemp)
			sequence.push_back(temp.insert(*x));
		set.swap(temp);
	}
}

int main()
{
	srand(time(nullptr));
	std::fstream out;
	out.open("C:\\Users\\admin\\Desktop\\in.txt");
	for (int k = 4; k < 206; k=k+2) {
		//clock_t t = clock();
		auto t1 = std::chrono::high_resolution_clock::now();
		Container C1(Tree(k+1));
		Container C2(Tree(k+1));

		C2.Subst(C1, 0);
		//C1.Output();
		//C2.Output();
		C2.Excl(C1);
		C2.Mul(3);

		Container A(Tree(k + 1));
		Container B(Tree(k + 1));
		Container C(Tree(k + 1));
		Container D(Tree(k + 1));
		Container E(Tree(k + 1));

		Container R = (A & B) ^ (C & D & E);
		auto t2 = std::chrono::high_resolution_clock::now();
		auto dt = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);

		//cout << "DT = " << (dt.count()) << endl;
		
		if (out.is_open()) {
			out << k + 1 << " " << dt.count() << std::endl;
		}
		else cout << "AAA";
	}
	
	return 0;
}
