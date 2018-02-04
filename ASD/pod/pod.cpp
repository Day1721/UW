//#include "pod1.h"

#include <iostream>

#define uint unsigned int
#define ulong unsigned long long

inline uint asInt(bool expr) {
    return (expr ? 1 : 0);
}

ulong tab[2][1000000];

uint input[1000000];

int main() {
    uint n;
    std::cin >> n;

    for (int i = 0; i < n; i++) {
        std::cin >> input[i];
    }

    ulong res = 0;

    tab[0][n-1] = asInt(input[n-1] == 3);
    tab[1][n-1] = asInt(input[n-1] == 2);
    for (int i = n-2; i >=0; i--) {
        tab[0][i] = tab[0][i+1] + asInt(input[i] == 3);
        tab[1][i] = tab[1][i+1] + asInt(input[i] == 2);
    }
    res += tab[0][0]; // 3's amount
    res += tab[1][0]; // 2's amount

    ulong tmp = 0;
    for (int i = 0; i < n-1; i++) {
        tmp += asInt(input[i] == 2)*tab[0][i+1];
        tmp += asInt(input[i] == 1)*(tab[0][i+1] + tab[1][i+1]);
    }
    res += tmp; // (1,2)'s, (1,3)'s & (2,3)'s amount


    tab[1][n-1] = 0;
    for (int i = n-2; i >= 0; i--) {
        tab[1][i] = tab[1][i+1] + asInt(input[i] == 2)*tab[0][i+1];
    }
    tab[0][n-1] = 0;
    for (int i = n-2; i >= 0; i--) {
        tab[0][i] = tab[0][i+1] + asInt(input[i] == 1)*tab[1][i+1];
    }

    res += tab[0][0]; //(1,2,3)'s amount

    for (int i = 0; i < n; i++) {
        res += asInt(input[i] == 1);    //1's amount
    }

    std::cout << res << std::endl;
}