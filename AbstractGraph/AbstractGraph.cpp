#include <iostream>
#include <vector>
#include <list>
#include <fstream>
#include <string>

const int inf = 1000000;

class AbstractGraph {
private:
    int n, m;
    std::vector<std::vector<int>> MatrixAdj;
public:

    AbstractGraph() { Input(); }

    AbstractGraph(int n) : n(n), m(rand() % (n* (n - 1) / 2)) {
        MatrixAdj.resize(n);
        for (int i = 0; i < n; i++) MatrixAdj[i].resize(n, inf);
        Generation(n, m);
    };

    ~AbstractGraph() {};

    void Input() {
        int choice;
        std::string fname;
        std::cout << "Do you want to read data from file or write by console? Enter 0 or 1" << std::endl;
        std::cin >> choice;
        if (!choice) {
            std::ifstream F;
            std::cout << "Enter filename: ";
            std::cin >> fname;
            F.open(fname, std::ios::in);
            F >> n >> m;

            MatrixAdj.resize(n);
            for (int i = 0; i < n; i++) {
                MatrixAdj[i].resize(n, inf);
                MatrixAdj[i][i] = 0;
            }

            for (int i = 0; i < m; i++) {
                int v, u, s;
                F >> v >> u >> s;
                MatrixAdj[v - 1][u - 1] = MatrixAdj[u - 1][v - 1] = s;
            }
            F.close();
        }
        else {
            std::cout << "Enter count of vertices and edges: ";
            std::cin >> n >> m;

            MatrixAdj.resize(n);
            for (int i = 0; i < n; i++) {
                MatrixAdj[i].resize(n, inf);
                MatrixAdj[i][i] = 0;
            }

            std::cout << "Enter numbers of start and final vertices of edges and lengths of the edges:\n";
            for (int i = 0; i < m; i++) {
                int v, u, s;
                std::cin >> v >> u >> s;
                MatrixAdj[v - 1][u - 1] = MatrixAdj[u - 1][v - 1] = s;
            }
        }

    }

    void Generation(int n, int m) {
        for (int i = 0; i < n; ++i) {
            for (int j = i + 1; j < n; ++j) {
                if (rand() % 2) {
                    int a = rand() % 100;
                    MatrixAdj[i][j] = a;
                    MatrixAdj[j][i] = a;
                }
            }
            MatrixAdj[i][i] = 0;
        }
    }

    void Output() {
        std::cout << "\n   | ";
        for (int i = 0; i < n; ++i) {
            std::cout.width(3);
            std::cout << i + 1 << " ";
        }
        std::cout << std::endl;

        for (int i = 0; i < 4 * n + 4; ++i) std::cout << "-";
        std::cout << std::endl;

        for (int i = 0; i < n; ++i) {
            std::cout.width(2);
            std::cout << i + 1 << " | ";
            for (int j = 0; j < n; ++j) {
                std::cout.width(3);
                if (MatrixAdj[i][j] == inf) 
                    std::cout << "inf" << " ";
                else 
                    std::cout << MatrixAdj[i][j] << " ";
            }
            std::cout << std::endl;
        }
    }

    friend std::vector<int> Dijkstra(AbstractGraph G, int s);
    friend std::vector<int> BellmanFord(AbstractGraph G, int s);
    friend std::vector<int> FloydWarshall(AbstractGraph G, int s);
    friend std::list<int> Path(AbstractGraph G, std::vector<int> A, int s, int f);
};

void Output(std::vector<int> A, std::list<int> B, int s, int f) {
    std::cout << "Distance:" << std::endl;
    for (int i = 0; i < A.capacity(); i++) {
        std::cout.width(3);
        std::cout << i + 1 << "|";
    }
    std::cout << std::endl;
    for (int i = 0; i < 4 * A.capacity(); ++i) std::cout << "-";
    std::cout << std::endl;
    for (auto& i : A) {
        std::cout.width(3);
        if (i == inf) 
            std::cout << "inf|";
        else
            std::cout << i << "|";
    }

    std::cout << std::endl;
    std::cout << "Shortest distance from start to final: ";
    if (A[f - 1] == inf) 
        std::cout << "inf";
    else 
        std::cout << A[f - 1];
    std::cout << std::endl;

    std::cout << "Path: ";

    for (auto& i : B)
        std::cout << i << " ";
    std::cout << std::endl << std::endl;
}

int main() {
    srand(time(0));
    //AbstractGraph G;
    AbstractGraph G(15);

    G.Output();

    int s, f;
    std::cout << "\nEnter numbers of start and final vertices: ";
    std::cin >> s >> f;

    std::cout << "\nDijkstra algorithm" << std::endl;
    std::vector<int> A = Dijkstra(G, s - 1);
    std::list<int> B = Path(G, A, s - 1, f - 1);
    Output(A, B, s, f);
    

    std::cout << "Bellman-Ford algorithm" << std::endl;
    std::vector<int> C = BellmanFord(G, s - 1);
    std::list<int> D = Path(G, C, s - 1, f - 1);
    Output(C, D, s, f);

    std::cout << "Floyd-Warshall algorithm" << std::endl;
    std::vector<int> E = FloydWarshall(G, s - 1);
    std::list<int> F = Path(G, E, s - 1, f - 1);
    Output(E, F, s, f);

    return 0;
}

std::vector<int> Dijkstra(AbstractGraph G, int s) {
    std::vector<int> D(G.MatrixAdj[s]);

    std::vector<bool> visited(G.n);
    visited[s] = 1;

    for (int i = 0; i < G.n - 1; i++) {
        int v = -1, minWeight = inf;

        for (int j = 0; j < G.n; j++)
            if (not visited[j] and D[j] <= minWeight) {
                v = j;
                minWeight = D[j];
            }
        visited[v] = 1;

        for (int u = 0; u < G.n; u++) {
            if (not visited[u]) {
                if (D[v] + G.MatrixAdj[v][u] < D[u])
                    D[u] = D[v] + G.MatrixAdj[v][u];
            }
        }
    }

    return D;
}

std::vector<int> BellmanFord(AbstractGraph G, int s) {
    std::vector<int> D(G.MatrixAdj[s]);

    for (int k = 0; k < G.n; k++)
        for (int v = 0; v < G.n; v++)
            if (v != s) {
                for (int u = 0; u < G.n; u++)
                    D[v] = std::min(D[v], D[u] + G.MatrixAdj[u][v]);
            }

    return D;
}

std::vector<int> FloydWarshall(AbstractGraph G, int s) {
    std::vector<std::vector<int>> D(G.MatrixAdj);

    for (int k = 0; k < G.n; k++)
        for (int v = 0; v < G.n; v++)
            if (D[v][k] != inf)
                for (int u = v + 1; u < G.n; u++)
                    D[u][v] = D[v][u] = std::min(D[v][u], D[v][k] + D[k][u]);

    return D[s];
}

std::list<int> Path(AbstractGraph G, std::vector<int> A, int s, int f) {
    std::list<int> B;

    std::vector<bool> visited(G.n);
    int v = f;
    B.push_front(v + 1);
    visited[v] = 1;

    while (v != s) {
        for (int u = 0; u < G.n; u++) {
            if (not visited[u] and A[u] == A[v] - G.MatrixAdj[v][u]) {
                visited[u] = 1;
                v = u;
                B.push_front(v + 1);
                break;
            }
        }
    }

    return B;
}

/*
std::vector<int>Dijkstra_old(AbstractGraph G, int s) {

    std::vector<int> distance(G.n);
    int count, index, i, u, m = s + 1;
    std::vector<bool> visited(G.n);
    for (i = 0; i < G.n; i++)
    {
        distance[i] = INT_MAX; visited[i] = false;
    }
    distance[s] = 0;
    for (count = 0; count < G.n - 1; count++)
    {
        int min = INT_MAX;
        for (i = 0; i < G.n; i++)
            if (!visited[i] && distance[i] <= min)
            {
                min = distance[i]; index = i;
            }
        u = index;
        visited[u] = true;
        for (i = 0; i < G.n; i++)
            if (!visited[i] && G.MatrixAdj[u][i] && distance[u] != INT_MAX &&
                distance[u] + G.MatrixAdj[u][i] < distance[i])
                distance[i] = distance[u] + G.MatrixAdj[u][i];
    }
    return distance;
}
*/