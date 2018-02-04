#include <iostream>
#include <vector>
#include <stack>

#define uint unsigned int

class OutOfRangeException {};

class Vertex {
private:
    bool _stayCar = false;
    std::vector<uint> _edges = std::vector<uint>();
public:
    void addEdge(uint to) {
        _edges.push_back(to);
    }

    bool stayCar() const {
        return  _stayCar;
    }

    void setCar() {
        _stayCar = true;
    }

    std::vector<uint> const& getNeighbours() const {
        return _edges;
    }
};

class Graph {
private:
    uint _size;
    std::vector<Vertex> _vertices;
public:
    Graph(uint size) : _size(size), _vertices(size) { }

    Vertex const& operator[](uint i) const {
        if (i > _size || i <= 0)
            throw OutOfRangeException();
        return _vertices[i - 1];
    }

    Vertex& get(uint i) {
        if (i > _size || i <= 0)
            throw OutOfRangeException();
        return _vertices[i - 1];
    }

    uint size() const {
        return _size;
    }
};

void dfs(Graph const& g, uint start, bool flags[]){
    std::stack<uint> stack;

    stack.push(start);
    while(!stack.empty()) {
        uint next = stack.top();
        stack.pop();
        flags[next] = true;
        if (g[next].stayCar()) continue;

        for (uint neighbour : g[next].getNeighbours()) {
            if (!flags[neighbour]) stack.push(neighbour);
        }
    }
}

int main () {
    std::ios_base::sync_with_stdio(false);
    unsigned int n, m;
    std::cin >> n >> m;

    Graph g(n);
    for (uint i = 1; i <= n; i++) {
        int tmp;
        std::cin >> tmp;
        if (tmp == 1) g.get(i).setCar();
    }

    for (uint i = 0; i < m; i++) {
        uint j,k;
        std::cin >> j >> k;
        g.get(j).addEdge(k);
        g.get(k).addEdge(j);
    }

    bool flags[n+1];
    for (int i = 1; i <= n; i++) {
        flags[i] = false;
    }

    dfs(g, 1, flags);
    for (int i = 1; i <= n; i++) {
        if (g[i].stayCar() && flags[i]) {
            std::cout << i << std::endl;
        }
    }
}