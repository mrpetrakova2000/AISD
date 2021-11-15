#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <vector>
#include <queue>

class Vertex {
private:
    char sym;
    int depth;
    Vertex* parent;
    std::vector<Vertex*> adjVertex; //contains parent
public:
    Vertex(int a = 0, Vertex* b = nullptr, char c = '!') : depth(a), parent(b), sym(c) {};
    friend class Tree;
};

class Tree {
private:
    int n;
    int maxDepth;
    Vertex* root;
public:
    Tree() : n(0), maxDepth(0), root(new Vertex()) { Input(root); std::cout << std::endl; };
    Tree(int a) : n(0), maxDepth(a) { root = Generation(); };

    Vertex* Generation(Vertex* V = nullptr);

    void Bfs();
    void BfsSearchParents();
    void Input(Vertex* V);
    void Output();
};

Vertex* Tree::Generation(Vertex* V) {
    int tmpDepth = (V) ? V->depth + 1 : 0;
    bool b = (tmpDepth < rand() % (maxDepth + 1));

    if (b) {
        Vertex* U = new Vertex(tmpDepth, V);
        for (int i = 0; i < 3; i++) {
            Vertex* W = Generation(U);
            U->adjVertex.push_back(W);
        }
        U->sym = 'a' + n++;
        return U;
    }
    return nullptr;
}

void Tree::Bfs() {
    std::cout << "Bfs:";

    std::queue<Vertex*> q;
    if (root) q.push(root);
    while (!q.empty()) {
        Vertex* V = q.front();
        q.pop();
        std::cout << " " << V->sym;
        if (V->parent) std::cout << "(" << V->parent->sym << ")";
        for (auto U = V->adjVertex.begin(); U != V->adjVertex.end(); U++)
            if (*U) q.push(*U);
    }
    std::cout << std::endl;
}

void Tree::BfsSearchParents() {
    std::vector<Vertex*> p;

    std::queue<Vertex*> q;
    if (root) q.push(root);
    while (!q.empty()) {
        Vertex* V = q.front();
        q.pop();

        if (V->adjVertex.size() > 1)
            p.push_back(V);

        for (auto U = V->adjVertex.begin(); U != V->adjVertex.end(); U++)
            if (*U) q.push(*U);
    }

    //std::cout << "The number of vertices with at least one descendant - ";
    std::cout << "The number of parents - " << p.size() << ": ";
    for (auto& i : p) std::cout << i->sym << " ";
    std::cout << std::endl;
}

void Tree::Input(Vertex* V) {
    bool b;

    std::cout << "Temporary depth is " << V->depth << std::endl;
    std::cout << "Left son?" << std::endl;
    std::cin >> b;
    if (b) {
        Vertex* U = new Vertex(V->depth + 1, V);
        Input(U);
        V->adjVertex.push_back(U);
    }

    std::cout << "Temporary depth is " << V->depth << std::endl;
    std::cout << "Middle son?" << std::endl;
    std::cin >> b;
    if (b) {
        Vertex* U = new Vertex(V->depth + 1, V);
        Input(U);
        V->adjVertex.push_back(U);
    }

    std::cout << "Temporary depth is " << V->depth << std::endl;
    std::cout << "Right son?" << std::endl;
    std::cin >> b;
    if (b) {
        Vertex* U = new Vertex(V->depth + 1, V);
        Input(U);
        V->adjVertex.push_back(U);
    }

    if (V->depth > maxDepth) maxDepth = V->depth;
    V->sym = 'a' + n++;
}

void Tree::Output() {
    std::queue<Vertex*> q;
    int tmpDepth = 0;
    if (root) {
        q.push(root);
        tmpDepth = root->depth;
    }

    while (!q.empty()) {
        Vertex* V = q.front();
        q.pop();
        if (tmpDepth != V->depth) {
            tmpDepth = V->depth;
            std::cout << std::endl;
        }
        std::cout << V->sym;
        if (V->parent) std::cout << "(" << V->parent->sym << ") ";
        for (auto U = V->adjVertex.begin(); U != V->adjVertex.end(); U++) 
            if (*U) q.push(*U);
    }

    std::cout << std::endl;
}


int main()
{
    //srand(time(0));

    Tree T(2); // initialization with arg - generation tree. Arg is max depth of tree
    // Tree T1; // initialization without arg - input
    T.Output(); // parent in brakets
    //T.Bfs();
    //T.BfsSearchParents();

    return 0;
}
