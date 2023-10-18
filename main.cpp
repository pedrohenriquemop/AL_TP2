#include <stdio.h>

#include <algorithm>
#include <iostream>
#include <set>
#include <vector>

using namespace std;

#define DEBUG false

class Aresta {
   public:
    int v1, v2, ano, tempo, custo;

    Aresta() : Aresta(-1, -1, 0, 0, 0){};

    Aresta(int _v1, int _v2, int _ano, int _tempo, int _custo) : v1(_v1), v2(_v2), ano(_ano), tempo(_tempo), custo(_custo){};
};

int findMinDistanceIndexTempo(int size, vector<long> &dist, vector<bool> &visited) {
    long minDist = 99999999999, minIndex = -1;

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

    return Aresta(v1, v2, 0, 0, 0);
}

void dijkstra(int src, int size, vector<Aresta> &arestas) {
    vector<long> dist(size, 99999999999);
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
    } else {
        for (int d : dist)
            cout << d << endl;
        cout << *max_element(anos.begin(), anos.end()) << endl;
    }
}

set<int> findAdjacentEdgesIds(int src, vector<Aresta> &arestas) {
    set<int> adj;

    for (int i = 0; i < arestas.size(); i++) {
        if (arestas[i].v1 == src || arestas[i].v2 == src) {
            adj.insert(i);
        }
    }

    return adj;
}

int findMinCustoArestaId(vector<Aresta> &arestas, set<int> &arestasIds) {
    int menor = 99999, menorId = -1;

    for (int id : arestasIds) {
        if (arestas[id].custo < menor) {
            menor = arestas[id].custo;
            menorId = id;
        }
    }

    return menorId;
}

int findMinAnoArestaId(vector<Aresta> &arestas, set<int> &arestasIds) {
    int menor = 99999999, menorId = -1;

    for (int id : arestasIds) {
        if (arestas[id].ano < menor) {
            menor = arestas[id].ano;
            menorId = id;
        }
    }

    return menorId;
}

bool contains(vector<int> &v, int x) {
    return std::find(v.begin(), v.end(), x) != v.end();
}

void prim(int src, int size, vector<Aresta> &arestas) {
    vector<Aresta> mst;
    long custoTotal = 0;
    vector<int> visited(size);
    visited.push_back(src);

    set<int> arestasIdsToPick;

    set<int> foundResult = findAdjacentEdgesIds(src, arestas);
    arestasIdsToPick.insert(foundResult.cbegin(), foundResult.cend());

    while (!arestasIdsToPick.empty()) {
        int minArestaId = findMinCustoArestaId(arestas, arestasIdsToPick);
        arestasIdsToPick.erase(minArestaId);

        Aresta currAresta = arestas[minArestaId];

        // if (DEBUG) {
        //     cout << "Prim visitando aresta: " << currAresta.v1 << " - " << currAresta.v2 << endl;
        //     cout << "Estado do set de arestas: " << endl;
        //     for (int s : arestasIdsToPick) {
        //         cout << s << " ";
        //     }
        //     cout << endl;
        // }

        if (!contains(visited, currAresta.v1)) {
            // if (DEBUG) {
            //     cout << "Entrou 1" << endl;
            // }
            mst.push_back(currAresta);
            visited.push_back(currAresta.v1);

            custoTotal += currAresta.custo;

            foundResult = findAdjacentEdgesIds(currAresta.v1, arestas);
            arestasIdsToPick.insert(foundResult.cbegin(), foundResult.cend());
        }
        if (!contains(visited, currAresta.v2)) {
            // if (DEBUG) {
            //     cout << "Entrou 2" << endl;
            // }
            mst.push_back(currAresta);
            visited.push_back(currAresta.v2);

            custoTotal += currAresta.custo;

            foundResult = findAdjacentEdgesIds(currAresta.v2, arestas);
            arestasIdsToPick.insert(foundResult.cbegin(), foundResult.cend());
        }
    }

    if (DEBUG) {
        cout << "Arestas Prim ( " << custoTotal << " ):" << endl;
        for (Aresta a : mst) {
            cout << a.v1 << " - " << a.v2 << ": " << a.custo << endl;
        }
    } else {
        cout << custoTotal << endl;
    }
}

void primAno(int src, int size, vector<Aresta> &arestas) {
    vector<Aresta> mst;
    int anoMax = 0;
    vector<int> visited(size);
    visited.push_back(src);

    set<int> arestasIdsToPick;

    set<int> foundResult = findAdjacentEdgesIds(src, arestas);
    arestasIdsToPick.insert(foundResult.cbegin(), foundResult.cend());

    while (!arestasIdsToPick.empty()) {
        int minArestaId = findMinAnoArestaId(arestas, arestasIdsToPick);
        arestasIdsToPick.erase(minArestaId);

        Aresta currAresta = arestas[minArestaId];

        if (!contains(visited, currAresta.v1)) {
            mst.push_back(currAresta);
            visited.push_back(currAresta.v1);

            if (currAresta.ano > anoMax) anoMax = currAresta.ano;

            foundResult = findAdjacentEdgesIds(currAresta.v1, arestas);
            arestasIdsToPick.insert(foundResult.cbegin(), foundResult.cend());
        }
        if (!contains(visited, currAresta.v2)) {
            mst.push_back(currAresta);
            visited.push_back(currAresta.v2);

            if (currAresta.ano > anoMax) anoMax = currAresta.ano;

            foundResult = findAdjacentEdgesIds(currAresta.v2, arestas);
            arestasIdsToPick.insert(foundResult.cbegin(), foundResult.cend());
        }
    }

    if (DEBUG) {
        cout << "Arestas Prim Ano ( " << anoMax << " ):" << endl;
    } else {
        cout << anoMax << endl;
    }
}

int main() {
    int n, m;
    vector<Aresta> arestas;

    scanf("%d%d", &n, &m);

    if (DEBUG) {
        cout << "N: " << n << ", M: " << m << endl;
    }

    for (int i = 0; i < m; i++) {
        int v1, v2, a, t, c;
        scanf("%d%d%d%d%d", &v1, &v2, &a, &t, &c);

        arestas.push_back(Aresta(v1 - 1, v2 - 1, a, t, c));
    }

    if (DEBUG) {
        cout << "Arestas:" << endl;
        for (int i = 0; i < arestas.size(); i++) {
            cout << "v1: " << arestas[i].v1 << ", v2: " << arestas[i].v2 << ", a: " << arestas[i].ano << ", t: " << arestas[i].tempo << ", c: " << arestas[i].custo << endl;
        }
    }

    dijkstra(0, n, arestas);
    primAno(0, n, arestas);
    prim(0, n, arestas);

    return 0;
};