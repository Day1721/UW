#include <iostream>
#include <algorithm>

using namespace std;

int main() {
    int m, n;
    cin >> n >> m;
    long long players[n];
    for(int i = 0; i < n; i++)
        players[i] = 0;
    for(int i = 0; i < m; i++)
        for(int j = 0; j < n; j++)
        {
            int player_number;
            cin >> player_number;
            if(j < n / 2) players[player_number - 1] += 1LL << i;
        }

    sort(players, players + n, greater<long long>());

    bool result = true;
    for(int i = 0; i < n - 1; i++)
        result &= (players[i] != players[i+1]);

    cout << (result ? "TAK" : "NIE");
}