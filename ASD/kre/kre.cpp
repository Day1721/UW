#include <iostream>

#define MaxNodes 1000000
#define MaxValue 2000000000

#define max(a,b) (((a) > (b)) ? (a) : (b))
#define min(a,b) (((a) < (b)) ? (a) : (b))

#define uint unsigned long long

struct Node{
    long long maxValue;
    long long minValue;
    uint leftBorder;
    uint rightBorder;
    uint increases;

    long long leftBorderValue;
    long long rightBorderValue;

    long long delta = 0;
};

Node nodes[MaxNodes];
uint months[MaxNodes/2];

void buildTree(unsigned int begin, unsigned int end, unsigned int index = 1) {
    if (begin != end) {
        //NODE
        nodes[index].delta = 0;
        nodes[index].leftBorder = begin;
        nodes[index].rightBorder = end;
        unsigned int mean = (begin + end) / 2;
        buildTree(begin, mean, 2*index);            //RECURSIVE CALLS !!!
        buildTree(mean + 1, end, 2*index + 1);      //TODO rec => iter
        nodes[index].minValue = min(nodes[2*index].minValue, nodes[2*index + 1].minValue);
        nodes[index].maxValue = max(nodes[2*index].maxValue, nodes[2*index + 1].maxValue);
        bool borderIncrease = months[mean] < months[mean + 1];
        nodes[index].increases = nodes[2*index].increases + nodes[2*index + 1].increases + borderIncrease;
        nodes[index].leftBorderValue = months[begin];
        nodes[index].rightBorderValue = months[end];
    }
    else {
        //LEAF
        nodes[index].delta = nodes[index].minValue = nodes[index].maxValue = nodes[index].leftBorderValue = nodes[index].rightBorderValue = months[begin];
        nodes[index].leftBorder = nodes[index].rightBorder = begin;
        nodes[index].increases = 0;
    }
}

bool checkTree(uint l, uint r, long long delta, uint index = 1) {
    Node node = nodes[index];
    if (node.leftBorder == l && node.rightBorder == r) {
        // checking all subtree => we already have all required data
        return delta + node.maxValue <= MaxValue && node.minValue >= (-delta);
    }
    uint nodeMean = (node.leftBorder + node.rightBorder) / 2;
    if (r <= nodeMean) {
        // here [l, r] \in [node.leftBorder, nodeMean] ~ "node.leftSubtree"
        return checkTree(l, r, delta + node.delta, 2*index);
    }
    if (l > nodeMean){
        // here [l, r] \in [nodeMean + 1, node.rightBorder] ~ "node.rightSubtree"
        return checkTree(l, r, delta + node.delta, 2*index + 1);
    }

    return checkTree(l, nodeMean, delta + node.delta, 2*index) && checkTree(nodeMean + 1, r, delta + node.delta, 2*index + 1);
}

uint updateTree(uint l, uint r, int delta, uint index = 1){
    uint nl = nodes[index].leftBorder;
    uint nr = nodes[index].rightBorder;
    if (nl == l && nr == r) {
        nodes[index].delta += delta;
        nodes[index].maxValue += delta;
        nodes[index].minValue += delta;
        nodes[index].leftBorderValue += delta;
        nodes[index].rightBorderValue += delta;

        return nodes[index].increases;
    }
    uint nodeMean = (nl + nr) / 2;
    if (r <= nodeMean) {
        uint leftIncs = updateTree(l, r, delta, 2*index);
        uint rightIncs = nodes[2*index + 1].increases;
        bool borderInc = nodes[2*index].rightBorderValue < nodes[2*index + 1].leftBorderValue;
        uint incs = leftIncs + rightIncs + borderInc;
        nodes[index].increases = incs;
        nodes[index].leftBorderValue = nodes[2*index].leftBorderValue + nodes[index].delta;
        nodes[index].minValue = min(nodes[2*index].minValue, nodes[2*index + 1].minValue) + nodes[index].delta;
        nodes[index].maxValue = max(nodes[2*index].maxValue, nodes[2*index + 1].maxValue) + nodes[index].delta;
        return incs;
    }
    if (l > nodeMean) {
        uint leftIncs = nodes[2*index].increases;
        uint rightIncs = updateTree(l, r, delta, 2*index + 1);
        bool borderInc = nodes[2*index].rightBorderValue < nodes[2*index + 1].leftBorderValue;
        uint incs = leftIncs + rightIncs + borderInc;
        nodes[index].increases = incs;
        nodes[index].rightBorderValue = nodes[2*index + 1].rightBorderValue + nodes[index].delta;
        nodes[index].minValue = min(nodes[2*index].minValue, nodes[2*index + 1].minValue) + nodes[index].delta;
        nodes[index].maxValue = max(nodes[2*index].maxValue, nodes[2*index + 1].maxValue) + nodes[index].delta;
        return incs;
    }

    uint leftIncs = updateTree(l, nodeMean, delta, 2*index);
    uint rightIncs = updateTree(nodeMean + 1, r, delta, 2*index + 1);
    bool borderInc = nodes[2*index].rightBorderValue < nodes[2*index + 1].leftBorderValue;
    uint incs = leftIncs + rightIncs + borderInc;
    nodes[index].increases = incs;
    nodes[index].leftBorderValue = nodes[2*index].leftBorderValue + nodes[index].delta;
    nodes[index].rightBorderValue = nodes[2*index + 1].rightBorderValue + nodes[index].delta;
    nodes[index].minValue = min(nodes[2*index].minValue, nodes[2*index + 1].minValue) + nodes[index].delta;
    nodes[index].maxValue = max(nodes[2*index].maxValue, nodes[2*index + 1].maxValue) + nodes[index].delta;
    return incs;
}

int main() {
    unsigned int n;
    std::cin >> n;

    for (int i = 0; i < n; i++) {
        std::cin >> months[i];
    }

    buildTree(0, n - 1);     //T(n) = O(n)
    int m;
    std::cin >> m;
    for (int i = 0; i < m; i++) {
        unsigned int l, r;
        long long delta;
        std::cin >> l >> r >> delta;

        l--;
        r--;

        if (-2000000000 > delta || delta > 2000000000) {
            std::cout << -1 << std::endl;
            continue;
        }

        int d = (int) delta;

        bool canUpdate = checkTree(l, r, delta);
        if (!canUpdate) {
            std::cout << -1 << std::endl;
            continue;
        }

        uint res = updateTree(l, r, d);
        std::cout << res << std::endl;
    }
}