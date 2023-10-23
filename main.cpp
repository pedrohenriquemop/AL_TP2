#include <stdio.h>

#include <algorithm>
#include <iostream>
#include <set>
#include <utility>
#include <vector>

using namespace std;

#define DEBUG false

class Edge {
   public:
    int v, ano, tempo, custo;

    Edge(int _v, int _ano, int _tempo, int _custo) : v(_v), ano(_ano), tempo(_tempo), custo(_custo){};
};

void dijkstra(int src, int size, vector<vector<Edge>> &adj) {
    set<pair<int, int>> s;  // guarda pares {tempo, verticeAdj}
    vector<long> dist(size, 1215752191);
    vector<int> anos(size, 0);

    s.insert({0, src});
    dist[src] = 0;

    while (!s.empty()) {
        pair<int, int> curr = *(s.begin());

        int currTempo = curr.first;
        int currVertice = curr.second;
        s.erase(curr);

        for (Edge edge : adj[currVertice]) {
            int verticeAdj = edge.v;
            int verticeTempo = edge.tempo;

            if (currTempo + verticeTempo < dist[verticeAdj]) {
                if (dist[verticeAdj] != 1215752191) {
                    s.erase({dist[verticeAdj], verticeAdj});
                }

                dist[verticeAdj] = currTempo + verticeTempo;
                s.insert({dist[verticeAdj], verticeAdj});

                anos[verticeAdj] = edge.ano;
            }
        }
    }

    for (int d : dist)
        cout << d << endl;
    cout << *max_element(anos.begin(), anos.end()) << endl;
}

void primAno(int src, int size, vector<vector<Edge>> &adj) {
    set<pair<int, int>> s;  // guarda pares {ano, verticeAdj}
    vector<bool> visited(size, false);
    int anoMax = 0;

    s.insert({0, src});

    while (!s.empty()) {
        pair<int, int> curr = *(s.begin());
        s.erase(curr);

        int currAno = curr.first;
        int currVertice = curr.second;

        if (visited[currVertice]) continue;

        visited[currVertice] = true;
        if (currAno > anoMax) anoMax = currAno;

        for (Edge edge : adj[currVertice]) {
            if (!visited[edge.v]) {
                s.insert({edge.ano, edge.v});
            }
        }
    }

    cout << anoMax << endl;
}

void primCusto(int src, int size, vector<vector<Edge>> &adj) {
    set<pair<int, int>> s;  // guarda pares {custo, verticeAdj}
    vector<bool> visited(size, false);
    long costSum = 0;

    s.insert({0, src});

    while (!s.empty()) {
        pair<int, int> curr = *(s.begin());
        s.erase(curr);

        int currVertice = curr.second;
        int currCusto = curr.first;

        if (visited[currVertice]) continue;

        visited[currVertice] = true;
        costSum += currCusto;

        for (Edge edge : adj[currVertice]) {
            if (!visited[edge.v]) {
                s.insert({edge.custo, edge.v});
            }
        }
    }

    cout << costSum << endl;
}

int main() {
    int n, m;

    scanf("%d%d", &n, &m);

    vector<vector<Edge>> adj(n);

    if (DEBUG) {
        cout << "N: " << n << ", M: " << m << endl;
    }

    for (int i = 0; i < m; i++) {
        int v1, v2, a, t, c;
        scanf("%d%d%d%d%d", &v1, &v2, &a, &t, &c);

        adj[v1 - 1].push_back(Edge(v2 - 1, a, t, c));
        adj[v2 - 1].push_back(Edge(v1 - 1, a, t, c));
    }

    if (DEBUG) {
        cout << "Adjacencias:" << endl;
        for (int i = 0; i < adj.size(); i++) {
            cout << i << ": ";
            for (int j = 0; j < adj[i].size(); j++) {
                cout << "v: " << adj[i][j].v << ", a: " << adj[i][j].ano << ", t: " << adj[i][j].tempo << ", c: " << adj[i][j].custo;
            }
            cout << endl;
        }
    }

    dijkstra(0, n, adj);
    primAno(0, n, adj);
    primCusto(0, n, adj);

    return 0;
};