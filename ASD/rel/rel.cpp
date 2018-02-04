#include <iostream>
using namespace std;

inline int max(int a, int b)
{
    return a > b ? a : b;
}

int array[502][42];
int computations[502][42][42];      //500*1600 = 800,000 (ints) <= 800,000 * 32 (B) <= 25 (MB)

int maximize2(int up, int down, int l, int n)    //działa w czasie stałym
{
    if(up == down)
        return 0;
    int max = 0;
    for(int k = -1; k <= 1; k++)
        for(int i = -2; i <= 2; i++)
            for(int j = -2; j <= 2; j++)
            {
                int oldUp = up + i + j;
                if(oldUp == down + k || oldUp < 0 || oldUp >= n || down + k < 0 || down + k >= n)
                    continue;
                //poprzednie wartości są różne i są w przedziale [0..n-1]

                int candidate = computations[l - 2][oldUp][down + k] + array[l][up] + array[l - 1][up + i] + array[l][down];
                max = (candidate > max) ? candidate : max;
            }
    return max;
};

int maximize3(int up, int down, int l, int n)
{
    int max = 0;
    for(int k = -1; k <= 1; k++)
        for(int i1 = -2; i1 <= 2; i1++)
            for(int i2 = -2; i2 <= 2; i2++)
                for(int i3 = -2; i3 <= 2; i3++)
                {
                    int oldUp = up + i1 + i2 + i3;
                    if(oldUp == down + k || oldUp < 0 || oldUp >= n || down + k < 0 || down + k >= n)
                        continue;

                    int candidate = computations[l - 3][oldUp][down + k] + array[l][up] + array[l - 1][up + i1] +
                            array[l - 2][up + i1 + i2] + array[l][down];
                    max = (candidate > max) ? candidate : max;
                }
    return max;
};


int main()
{
    int m, n;                               //3 <= m <= 500, 2 <= n <= 40
    cin >> m >> n;                       //foreach(i,j in {0..n-1}) array[i][j] <= 10^5
    for(int i = m - 1; i >= 0; i--)
        for(int j = 0; j < n; j++)
            cin >> array[i][j];

    //wypełniamy pierwsze 3 "wiersze"
    for(int up = 0; up < n; up++)
        for(int down = 0; down < n; down++)
        {
            computations[0][up][down] = array[0][up] + array[0][down];
            if(up == down) computations[0][up][down] = 0;
            computations[1][up][down] = 0;
        }

    for(int up = 0; up < n; up++)
        for(int down = 0; down < n; down++)
        {
            if(up == down)
            {
                computations[2][up][down] = 0;
                continue;
            }
            computations[2][up][down] = maximize2(up, down, 2, n);
        }

    for(int l = 3; l < m; l++)
        for(int up = 0; up < n; up++)
            for(int down = 0; down < n; down++)
            {
                if(up == down)
                {
                    computations[l][up][down] = 0;
                    continue;
                }
                int max2 = maximize2(up, down, l, n);
                int max3 = maximize3(up, down, l, n);
                computations[l][up][down] = max(max2, max3);
            }

    int res = 0;
    for(int i = 0; i < n; i++)
        for(int j = 0; j < n; j++)
            res = max(res, computations[m - 1][i][j]);

    cout << res;
}