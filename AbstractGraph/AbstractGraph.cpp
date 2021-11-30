#include <iostream>
#include <vector>
#include <list>

class AbstractGraph {
private:
    int n, m;
    std::vector<std::vector<int>> MatrixAdj;
public:
    void Input() {
        std::cin >> n >> m;

        MatrixAdj.resize(n);
        for (int i = 0; i < n; i++) MatrixAdj[i].resize(n);

        for (int i = 0; i < m; i++) {
            int v, u, s;
            std::cin >> v >> u >> s;
            MatrixAdj[v - 1][u - 1] = MatrixAdj[u - 1][v - 1] = s;
        }
    }

    void Output() {
        std::cout << std::endl;
        for (auto& i : MatrixAdj) {
            for (auto& j : i)
                std::cout << j << " ";
            std::cout << std::endl;
        }
    }

    friend std::vector<int> Dijkstra(AbstractGraph G, int s);
    friend std::list<int> Path(AbstractGraph G, std::vector<int> A, int s, int f);
};

int main() {

    AbstractGraph G;

    G.Input();
    G.Output();

    int s = 1, f = 6;

    std::cout << "Distance: ";
    std::vector<int> A = Dijkstra(G, s - 1);
    for (auto& i : A)
        std::cout << i << " ";
    std::cout << std::endl;

    std::cout << "Path: ";
    std::list<int> B = Path(G, A, s - 1, f - 1);
    for (auto& i : B)
        std::cout << i << " ";

    return 0;
}

std::vector<int> Dijkstra(AbstractGraph G, int s) {
    std::vector<int> D(G.n);
    for (int v = 0; v < G.n; v++) {
        if (G.MatrixAdj[s][v])
            D[v] = G.MatrixAdj[s][v];
        else
            D[v] = 100000;
    }
    D[s] = 0;

    std::vector<bool> visited(G.n);
    visited[s] = 1;

    for (int i = 0; i < G.n; i++) {
        int v, minWeight = 1000000;

        for (int j = 0; j < G.n; j++)
            if (not visited[j] and D[j] < minWeight) {
                v = j;
                minWeight = D[j];
            }
        visited[v] = 1;

        for (int u = 0; u < G.n; u++) {
            if (not visited[u] and G.MatrixAdj[v][u]) {
                if (D[v] + G.MatrixAdj[v][u] < D[u])
                    D[u] = D[v] + G.MatrixAdj[v][u];
            }
        }
    }

    return D;
}

std::list<int> Path(AbstractGraph G, std::vector<int> A, int s, int f) {
    std::list<int> B;

    int v = f;
    B.push_front(v + 1);

    while (v != s) {
        for (int u = 0; u < G.n; u++) {
            if (G.MatrixAdj[v][u])
                if (A[u] == A[v] - G.MatrixAdj[v][u]) {
                    v = u;
                    B.push_front(v + 1);
                    break;
                }
        }
    }

    return B;
}
