#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;

#define DEBUG true

class Aresta {
   public:
    int v1, v2, ano, tempo, custo;

    Aresta(int _v1, int _v2) : v1(_v1), v2(_v2), ano(0), tempo(0), custo(0) {}

    Aresta(int _v1, int _v2, int _ano, int _tempo, int _custo) : v1(_v1), v2(_v2), ano(_ano), tempo(_tempo), custo(_custo){};
};

int findMinDistanceIndexTempo(int size, vector<long> &dist, vector<bool> &visited) {
    long minDist = LONG_MAX, minIndex = -1;

    for (int i = 0; i < size; i++) {
        if (!visited[i] && dist[i] < minDist) {
            minDist = dist[i];
            minIndex = i;
        }
    }

    return minIndex;
}

vector<int> findAdjacentVertices(int src, vector<Aresta> &arestas) {
    vector<int> adjacentVertices;

    for (int i = 0; i < arestas.size(); i++) {
        if (arestas[i].v1 == src) {
            adjacentVertices.push_back(arestas[i].v2);
        } else if (arestas[i].v2 == src) {
            adjacentVertices.push_back(arestas[i].v1);
        }
    }

    return adjacentVertices;
}

Aresta findAresta(int v1, int v2, vector<Aresta> &arestas) {
    for (int i = 0; i < arestas.size(); i++) {
        if (arestas[i].v1 == v1 && arestas[i].v2 == v2 || arestas[i].v1 == v2 && arestas[i].v2 == v1) {
            return arestas[i];
        }
    }

    return Aresta(v1, v2);
}

void dijkstraTempo(int size, int src, vector<Aresta> &arestas) {
    vector<long> dist(size, LONG_MAX);
    vector<bool> visited(size, false);
    vector<int> anos(size, 0);

    dist[src] = 0;
    for (int i = 0; i < size; i++) {
        int u = findMinDistanceIndexTempo(size, dist, visited);
        visited[u] = true;

        vector<int> adjacentVertices = findAdjacentVertices(u, arestas);

        for (int v : adjacentVertices) {
            Aresta a = findAresta(u, v, arestas);
            if (!visited[v] && dist[u] + a.tempo < dist[v]) {
                dist[v] = dist[u] + a.tempo;
                anos[v] = a.ano;
            }
        }
    }

    if (DEBUG) {
        for (int i = 0; i < dist.size(); i++)
            cout << "dist[" << i << "]: " << dist[i] << endl;

        cout << "Ano max: " << *max_element(anos.begin(), anos.end()) << endl;
    }
}

int main() {
    int n, m;
    vector<Aresta> arestas;

    ios::sync_with_stdio(false);  // usado para tornar a performance da iostream do C++ melhor que o scanf

    cin >> n >> m;

    if (DEBUG) {
        cout << "N: " << n << ", M: " << m << endl;
    }

    for (int i = 0; i < m; i++) {
        int v1, v2, a, t, c;
        cin >> v1 >> v2 >> a >> t >> c;

        arestas.push_back(Aresta(v1 - 1, v2 - 1, a, t, c));
    }

    if (DEBUG) {
        cout << "Arestas:" << endl;
        for (int i = 0; i < arestas.size(); i++) {
            cout << "v1: " << arestas[i].v1 << ", v2: " << arestas[i].v2 << ", a: " << arestas[i].ano << ", t: " << arestas[i].tempo << ", c: " << arestas[i].custo << endl;
        }
    }

    dijkstraTempo(n, 0, arestas);

    return 0;
};