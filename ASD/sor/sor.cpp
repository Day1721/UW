#include <iostream>

int main()
{   long long modulo = 1000 * 1000 * 1000;
    int n;
    std::cin >> n;
    long long values[n];
    for(int i = 0; i < n; i++)
    {
        std::cin >> values[i];
    }

    if(n == 1)
    {
        std::cout << "1";
        return 0;
    }
    
    long long left_first[n][n];
    long long right_first[n][n];

    for(int i = 0; i < n; i++)
    {
        left_first[i][i] = right_first[i][i] = 1;
    }

    for(int j = 1; j < n; j++)
        for(int x = 0, y = j; y < n; x++, y++)
        {
            left_first[x][y] = ((values[x + 1] > values[x] ? left_first[x + 1][y] : 0)
                    + (values[y] > values[x] ? right_first[x + 1][y] : 0)) % (modulo * 100);

            right_first[x][y] = ((values[x] < values[y] ? left_first[x][y - 1] : 0)
                    + (values[y - 1] < values[y] ? right_first[x][y - 1] : 0)) % (modulo * 100);
        }
        
    std::cout << (left_first[0][n - 1] / 2 + right_first[0][n - 1] / 2) % modulo;
}