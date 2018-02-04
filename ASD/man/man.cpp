#include <iostream>
#include <algorithm>
#include <vector>
#include <tuple>

using namespace std;

struct Tree {
    static const int power = 1048576;
    pair<int, int> arr[2 * power + 1];
/*
    int max, id;
    Tree* left;
    Tree* right;
    Tree* parent;
*/
    Tree()
    {
        for(int i = 1; i <= power; i++)
            arr[i] = make_pair(0, -1);
        for(int i = power + 1; i <= 2 * power; i++)
            arr[i] = make_pair(0, power - i);
    }

    void Update(int id, int sgn)
    {
        id += power;
        arr[id].first += sgn;
        while (id > 0)
        {
            id /= 2;
            arr[id] = max(arr[2 * id], arr[2 * id + 1]);
        }
    }

    pair<int, int> Query()
    {
        return arr[1];
    };
};

int main()
{
    int m, n;
    cin >> m >> n;

    pair<int, int> begins[m], ends[m];
    for(int i = 0; i < m; i++)
    {
        int b, e, mm;
        cin >> b >> e >> mm;
        begins[i] = pair<int, int>(b, mm);
        ends[i] = pair<int, int>(e, mm);
    }

    Tree* tree = new Tree();

    sort(begins, begins + m);
    sort(ends, ends + m);

    pair<int, int> queries[n];
    for(int i = 0; i < n; i++)
    {
        int a;
        cin >> a;
        queries[i] = make_pair(a, i);
    }

    sort(queries, queries + n);

    auto last = (ends + m - 1)->first;
    int i = 0, j = 0, k = 0;
    vector<tuple<int, int, int>> results;   //2 z pary i wynik
    for(int t = 1; t <= last; t++)
    {
        while(i < m && begins[i].first <= t)
        {
            tree->Update(begins[i].second, 1);
            i++;
        }
        while(k < n && queries[k].first == t)
        {
            results.push_back(make_tuple(queries[k].second, tree->Query().second, tree->Query().first));
            k++;
        }

        while(j < m && ends[j].first <= t)
        {
            tree->Update(ends[j].second, -1);
            j++;
        }
    }

    sort(results.begin(), results.end());

    for(auto res : results)
        cout << -get<1>(res) << " " << get<2>(res) << endl;

    delete(tree);
}