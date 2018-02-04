#include<iostream>
#include <vector>

#define uint unsigned int
#define ulong unsigned long long


class Node {
private:
    std::vector<uint> _neighbours = std::vector<uint>();
public:
    uint size = 1;
    ulong sum = 0;

    void addEdge(uint to) {
        _neighbours.push_back(to);
    }

    std::vector<uint> const& getNeighbours() const {
        return _neighbours;
    }


};


class Tree {
private:
    std::vector<Node> _nodes;
    uint _size;

    void _compute(uint node, bool flags[]) {
        flags[node-1] = true;
        int parent = -1;
        for (uint neighbour : _nodes[node-1].getNeighbours()) {
            if (!flags[neighbour-1]) _compute(neighbour, flags);
            else parent = neighbour;
        }

        //HERE we computed "all" for "childrens"
        _nodes[node-1].size = 1;
        _nodes[node-1].sum = 0;
        for (uint neighbour : _nodes[node-1].getNeighbours()) {
            if (neighbour != parent) {
                _nodes[node-1].size += _nodes[neighbour-1].size;
                _nodes[node-1].sum += _nodes[neighbour-1].size + _nodes[neighbour-1].sum;
            }
        }
    }

    void _correct(uint node, uint parent) {
        if (node != parent) {
            // Firstly correct yourself
            ulong outerSum = _nodes[parent-1].sum - (_nodes[node-1].sum + _nodes[node-1].size);
            outerSum += (_size - _nodes[node-1].size);
            _nodes[node-1].sum += outerSum;
        }
        for (uint neighbour : _nodes[node-1].getNeighbours()) {
            if (neighbour != parent) _correct(neighbour, node);
        }
    }

public:
    explicit Tree(uint size) : _size(size), _nodes(size) { }

    void addEdge(uint p, uint k) {
        _nodes[p-1].addEdge(k);
        _nodes[k-1].addEdge(p);
    }

    void compute() {
        bool flags[_size];
        for (int i = 0; i < _size; i++) {
            flags[i] = false;
        }
        _compute(1, flags);
        _correct(1, 1);
    }

    std::vector<Node> const& nodes() const {
        return _nodes;
    }
};


int main() {
    uint n;
    std::cin >> n;

    Tree tree(n);

    for (int i = 0; i < n-1; i++) {
        uint p, k;
        std::cin >> p >> k;
        tree.addEdge(p, k);
    }

    tree.compute();
    auto nodes = tree.nodes();
    for (int i = 0; i < n; i++) {
        std::cout << nodes[i].sum << std::endl;
    }
}