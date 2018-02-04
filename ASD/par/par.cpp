#include <iostream>
#include <cmath>
#include <cassert>

using namespace std;

int Left[500001];
int Right[500001];
int parent[500001];
int depth[500001];
int links[20][500001];
pair<int, int> farDown[500001];
pair<int, int> farUp[500001];
pair<int, int> Far[500001];
int n;

void fillDepth(int i, int prev) {
    if(i == -1) return;
    depth[i] = prev + 1;
    fillDepth(Left[i], prev + 1);
    fillDepth(Right[i], prev + 1);
}

int ancestor(int v, int h) {
    int res = v;
    int i = (int)floor(log(n));
    while (h > 0) {
        int power = (1 << i);
        if(power > h) i--;
        else {
            res = links[i][res];
            h -= power;
        }
    }
    return res == 0 ? 1 : res;
}

int lca(int u, int v) {
    int du = depth[u];
    int dv = depth[v];
    if(du < dv) {
        v = ancestor(v, dv - du);
        dv = depth[v];
    }
    else if(du > dv) {
        u = ancestor(u, du - dv);
        du = depth[u];
    }
    assert(du == dv);
    if(u == v) return u;
    int i = (int)floor(log(n));
    while(i >= 0) {
        if(links[i][u] != links[i][v]) {
            u = links[i][u];
            v = links[i][v];
        }
        i--;
    }
    return parent[u];
}

void fillFarDown(int i) {
    if(i == -1) return;
    fillFarDown(Left[i]);
    fillFarDown(Right[i]);

    pair<int, int> c1, c2, c3;
    c1 = make_pair(0, i);
    if(Left[i] != -1) {
        auto p = farDown[Left[i]];
        c2 = make_pair(p.first + 1, p.second);
    }
    else c2 = make_pair(-1, -1);

    if(Right[i] != -1) {
        auto p = farDown[Right[i]];
        c3 = make_pair(p.first + 1, p.second);
    }
    else c3 = make_pair(-1, -1);

    if(c1.first >= c2.first && c1.first >= c3.first)
        farDown[i] = c1;
    else if(c2.first >= c1.first && c2.first >= c3.first)
        farDown[i] = c2;
    else if(c3.first >= c2.first && c3.first >= c1.first)
        farDown[i] = c3;
}

void fillFarUp(int i) {
    if(i == -1) return;

    pair<int, int> c1, c2, c3;
    c1 = make_pair(0, i);
    if(parent[i] != -1) {
        auto p = farUp[parent[i]];
        c2 = make_pair(p.first + 1, p.second);

        if(Left[parent[i]] == i) {
            if(Right[parent[i]] != -1) {
                auto p2 = farDown[Right[parent[i]]];
                c3 = make_pair(p2.first + 2, p2.second);
            }
            else c3 = make_pair(-1, -1);
        }
        else {
            if(Left[parent[i]] != -1) {
                auto p2 = farDown[Left[parent[i]]];
                c3 = make_pair(p2.first + 2, p2.second);
            }
            else c3 = make_pair(-1, -1);
        }
    }
    else c2 = c3 = make_pair(-1, -1);

    if(c1.first >= c2.first && c1.first >= c3.first)
        farUp[i] = c1;
    else if(c2.first >= c1.first && c2.first >= c3.first)
        farUp[i] = c2;
    else if(c3.first >= c2.first && c3.first >= c1.first)
        farUp[i] = c3;

    fillFarUp(Left[i]);
    fillFarUp(Right[i]);
}

void fillFar() {
    fillFarDown(1);
    fillFarUp(1);
    for(int i = 1; i <= n; i++) {
        Far[i] = farDown[i].first > farUp[i].first ? farDown[i] : farUp[i];
    }
}

int dist(int u, int d) {
    auto p = Far[u];
    if(d > p.first) return -1;
    int l = lca(u, p.second);
    int d1 = depth[u] - depth[l];
    //int d2 = depth[p.second] - depth[l];
    if(d <= d1) return ancestor(u, d);
    return ancestor(p.second, p.first - d);
}

int main() {
    cin >> n;

    for (int i = 1; i <= n; i++) {
        int l, r;
        cin >> l >> r;
        Left[i] = l;
        Right[i] = r;

        parent[l] = i;
        parent[r] = i;
    }

    parent[1] = -1;

    fillDepth(1, -1);

    for (int v = 1; v <= n; v++)
        links[0][v] = parent[v];

    for (int i = 1; i <= floor(log(n)); i++)
        for (int v = 1; v <= n; v++) {
            if(links[i - 1][v] == -1) links[i][v] = -1;
            else links[i][v] = links[i - 1][links[i - 1][v]];
        }

    fillFar();

    int m;
    cin >> m;
    for (int i = 0; i < m; i++) {
        int u, d;
        cin >> u >> d;
        cout << dist(u, d) << endl;
    }
}