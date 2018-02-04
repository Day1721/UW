#include <iostream>
#include <vector>
#include <queue>

using namespace std;

struct Vertex
{
    int Value;
    vector<Vertex*> Ascendants;
    int DescendantsAmount;

    Vertex(int value) : Value(value), DescendantsAmount(0)
    { }

    void AddAscendant(Vertex* vertex)
    {
        Ascendants.push_back(vertex);
        vertex->DescendantsAmount++;
    }
};

struct Comparator
{
    bool operator()(Vertex* v1, Vertex* v2)
    {
        return v1->Value > v2->Value;
    }
};

int main()
{
    ios_base::sync_with_stdio(false);
    int n, m, k;
    scanf("%d %d %d", &n, &m, &k);

    Vertex* vertices[n];

    int values[n];
    int value;
    for(int i = 0; i < n; i++)
    {
        scanf("%d", &value);
        vertices[i] = new Vertex(value);
    }

    int a, b;
    for(int i = 0; i < m; i++)
    {
        scanf("%d %d", &a, &b);
        vertices[b - 1]->AddAscendant(vertices[a - 1]);
    }

    priority_queue<Vertex*, std::vector<Vertex*>, Comparator> priorityQueue;

    for(Vertex* v : vertices)
        if(v->DescendantsAmount == 0) priorityQueue.push(v);

    int result = 0;
    while (!priorityQueue.empty() && k > 0)
    {
        Vertex* vertex = priorityQueue.top();
        priorityQueue.pop();
        result = max(result, vertex->Value);
        k--;
        for(Vertex* v : vertex->Ascendants)
        {
            v->DescendantsAmount--;
            if(v->DescendantsAmount == 0)
                priorityQueue.push(v);
        }
    }

    printf("%d", result);
}