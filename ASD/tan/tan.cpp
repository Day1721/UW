#include <iostream>
#include <queue>
#include <set>
#include <tuple>
#include <climits>

inline int index(int n, int v, int l) {
    return l*n + v;
}

inline long long min(long long a, long long b) {
    return a > b ? b : a;
}

const int MaxN = 20000;
const int MaxK = 10;

class KeyValuePair {
public:
    int key;
    long long value;

    KeyValuePair(int key, long long value) : key(key), value(value) {}
};

class KvpCompare {
public:
    bool operator()(KeyValuePair const& left, KeyValuePair const& right){
        return left.value > right.value;
    }
};

class Edge {
public:
    int from;
    int to;
    int cost;

    Edge(int from, int to, int cost) : from(from), to(to), cost(cost) {}
};

class Vertex {
private:
    std::vector<Edge> _edges = std::vector<Edge>();

public:
    void addEdge(Edge edge) {
        _edges.push_back(edge);
    }

    std::vector<Edge> const& getEdges() const {
        return _edges;
    }
};

class Graph {
private:
    std::vector<Vertex> _vertices;
    int _size;

public:
    Graph(unsigned int size) : _size(size), _vertices(size) { }

    Vertex const& operator[](int i) const {
        if (i > _size || i < 0)
            throw "Invalid i in [] operator";
        return _vertices[i];
    }

    void addEdge(int from, int to, int cost) {
        _vertices[from].addEdge(Edge(from, to, cost));
    }
};

long long result[MaxN*(MaxK + 1)];

bool dijkstra(Graph const& g, int n, int fst){
    std::priority_queue<KeyValuePair, std::vector<KeyValuePair>, KvpCompare> queue;
    queue.push(KeyValuePair(fst, 0));
    bool res = false;
    while (!queue.empty()) {
        auto kvp = queue.top();
        queue.pop();
        if (result[kvp.key] < LLONG_MAX) continue;

        result[kvp.key] = kvp.value;
        res = res || (kvp.key % n) == n - 1;

        for (Edge const& e : g[kvp.key].getEdges()) {
            queue.push(KeyValuePair(e.to, kvp.value + e.cost));
        }
    }
    return res;
}

int main() {
    unsigned int n, m, k;
    std::cin >> n >> m >> k;
    Graph g(n*(k + 1));
    for (int i = 0; i < m; i++) {
        int v, w, b, c;
        std::cin >> v >> w >> b >> c;
        g.addEdge(v, w, c);
        for (int l = 1; l <= k; l++) {
            g.addEdge(index(n, v, l), index(n, w, l), c);
            g.addEdge(index(n, v, l), index(n, w, l - 1), c - b);
        }
    }
    for (int i = 0; i < n*(k + 1); i++) {
        result[i] = LLONG_MAX;
    }
    bool flag = dijkstra(g, n, index(n, 0, k));
    long long res = result[index(n, n - 1, 0)];
    for (int i = 1; i <= k; i++) {
        res = min(res, result[index(n, n - 1, i)]);
    }
    std::cout << (flag ? res : -1);
    return 0;
}