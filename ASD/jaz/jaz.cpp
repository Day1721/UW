#include <iostream>
#include <vector>
#include <stack>

using namespace std;

int n, m;

int main() {
    scanf("%d %d", &n, &m);
    vector<int> edges[n + 1];
    for(int i = 0; i < m; i++) {
        int u, v;
        scanf("%d %d", &u, &v);
        edges[u].push_back(v);
        edges[v].push_back(u);
    }

    bool hadFound = false;

    bool isChecked[n + 1];
    bool isVisited[n + 1];
    for(int i = 0; i <= n; i++)
        isChecked[i] = isVisited[i] = false;

    stack<int> s;
    for(int i = 1; i <= n; i++) {
        if(isChecked[i]) continue;

        s.push(i);
        isChecked[i] = true;
        while(!s.empty()) {
            int u = s.top();
            s.pop();
            isVisited[u] = true;
            for(int v : edges[u]) {
                if(isChecked[v] && !isVisited[v]) {
                    hadFound = true;
                    break;
                }

                else if(!isChecked[v]) {
                    isChecked[v] = true;
                    s.push(v);
                }
            }
        }
        if(hadFound) break;
    }

    printf("%s", (hadFound ? "TAK" : "NIE"));
}