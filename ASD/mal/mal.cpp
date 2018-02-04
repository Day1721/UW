#include <iostream>

using namespace std;

enum Color
{
    White, Black, Mixed
};

class Tree
{
public:
    int sum;
    int left;
    int right;
    Color color;
    Tree* leftChild;
    Tree* rightChild;

    Tree(Color color, int left, int right)
    {
        if(color == Color::Mixed) throw "Illegal argument";
        this->color = color;
        this->left = left;
        this->right = right;
        leftChild = rightChild = nullptr;
        sum = color == Color::Black ? 0 : right - left + 1;
    }

    int UpdateWhite(int begin, int end)
    {
        if(end < left || right < begin) return sum;
        if(begin <= left && end >= right)
        {
            color = Color::White;
            sum = right - left + 1;
            return sum;
        }
        if(color == Color::White && begin >= left && end <= right) return sum;
        if(color != Color::Mixed)
        {
            if(leftChild != nullptr) delete(leftChild);
            leftChild = new Tree(color, left, (left + right) / 2);
            if(rightChild != nullptr) delete(rightChild);
            rightChild = new Tree(color, (left + right) / 2 + 1, right);
            color = Color::Mixed;
        }

        sum = leftChild->UpdateWhite(begin, end) + rightChild->UpdateWhite(begin, end);
        return sum;
    }

    int UpdateBlack(int begin, int end)
    {
        if(end < left || right < begin) return sum;
        if(begin <= left && end >= right)
        {
            color = Color::Black;
            sum = 0;
            return sum;
        }
        if(color == Color::Black && begin >= left && end <= right) return sum;
        if(color != Color::Mixed)
        {
            if(leftChild != nullptr) delete(leftChild);
            leftChild = new Tree(color, left, (left + right) / 2);
            if(rightChild != nullptr) delete(rightChild);
            rightChild = new Tree(color, (left + right) / 2 + 1, right);
            color = Color::Mixed;
        }
        sum = leftChild->UpdateBlack(begin, end) + rightChild->UpdateBlack(begin, end);
        return sum;
    }
};

int main()
{
    int n, m;
    cin >> n;
    cin >> m;
    Tree* tree = new Tree(Color::Black, 1, n);
    for(int i = 0; i < m; i++)
    {
        int a, b;
        char c;
        cin >> a >> b >> c;
        int sum;
        if(c == 'C')
            sum = tree->UpdateBlack(a, b);
        else sum = tree->UpdateWhite(a, b);

        cout << sum << endl;
    }
}