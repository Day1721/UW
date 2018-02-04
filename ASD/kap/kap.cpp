#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <utility>
#include <climits>

using namespace std;

const int N = 200002;

pair<int, int> islands[N];
pair<int, int> xp[N];
pair<int, int> yp[N];
vector<int> neighbors[N]; 
priority_queue<std::pair<int, int>> pq;
int dist[N];
bool visited[N]; 

int main() 
{
	int n;
	std::cin >> n;
	for (int i = 1; i <= n; ++i) 
	{
		int x,y;
		std::cin >> x >> y;
		islands[i] = make_pair(x,y);
		xp[i] = make_pair(x, i);
		yp[i] = make_pair(y, i);
		dist[i] = INT_MAX;
		visited[i] = false;
	} 

	std::sort(xp + 1, xp + n + 1);
	std::sort(yp + 1, yp + n + 1);

	for (int i = 1; i < n; i++) 
	{
		neighbors[xp[i].second].push_back(xp[i + 1].second);
		neighbors[yp[i].second].push_back(yp[i + 1].second);
		neighbors[xp[i + 1].second].push_back(xp[i].second);
		neighbors[yp[i + 1].second].push_back(yp[i].second);
	}

	pq.push(make_pair(0, 1));
	while (!pq.empty()) 
	{
		std::pair<int, int> u = pq.top();
		pq.pop();
		
		if (!visited[u.second]) 
		{
			visited[u.second] = true;
			for (auto v : neighbors[u.second]) 
			{
				int tmp1 = abs(islands[u.second].first - islands[v].first);
				int tmp2 = abs(islands[u.second].second - islands[v].second);
				int w = min(tmp1, tmp2) - u.first;
				
				if (dist[v] > w) 
				{
				    pq.push(make_pair(-w, v));
				    dist[v] = w;
				}
			}
		} 
	}
	cout << dist[n] << endl;
}
