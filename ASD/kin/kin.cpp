#include <iostream>
using namespace std;

const int modulo = 1000 * 1000 * 1000;

class Tree
{
public:
    int left;
    int right;
    int value;
    Tree* leftTree;
    Tree* rightTree;

    Tree(int array[], int begin, int end)
    {
        if(begin == end)
        {
            left = right = begin;
            value = array[begin];
            leftTree = rightTree = nullptr;
            return;
        }

        left = begin;
        right = end;
        leftTree = new Tree(array, begin, (end + begin) / 2);
        rightTree = new Tree(array, (end + begin) / 2 + 1, end);
        value = leftTree->value + rightTree->value;
    }

    int Query(int begin, int end)
    {
        if(begin > right || end < left) return 0;
        if(begin <= left && end >= right) return value;
        return (leftTree->Query(begin, end) + rightTree->Query(begin, end)) % modulo;
    }

    void Update(int index, int newValue)
    {
        if(left == right)
        {
            value = newValue;
            return;
        }

        if(index <= (right + left) / 2)
            leftTree->Update(index, newValue);
        else rightTree->Update(index, newValue);

        value = (leftTree->value + rightTree->value) % modulo;
    }
};

int main()
{
    int n, k;
    cin >> n >> k;
    int values[n];
    for(int i = 0; i < n; i++)
        cin >> values[i];

    Tree* trees[k + 1];
    int zeros[n + 1];
    for(int i = 0; i <= n; i++)
        zeros[i] = 0;

    for(int i = 1; i <= k; i++)
    {
        trees[i] = new Tree(zeros, 0, n);
    }

    for(int i = 0; i < n; i++)
    {
        int x = values[i];
        trees[1]->Update(x, 1);
        for(int j = 2; j <= k; j++)
            trees[j]->Update(x, trees[j - 1]->Query(x + 1, n));
    }

    cout << trees[k]->value;
}