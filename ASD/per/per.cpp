#include <iostream>

#define uint unsigned int
#define ulong unsigned long long
#define MaxN 100000

inline uint max(uint a, uint b) {
    return a > b ? a : b;
}

inline ulong max(ulong a, ulong b) {
    return a > b ? a : b;
}

uint values[MaxN];
ulong cant[MaxN];
ulong can[MaxN];

int main() {
    uint n;
    std::cin >> n;

    for (uint i = 0; i < n; i++) {
        std::cin >> values[i];
    }

    can[n-1] = cant[n-1] = values[n-1];
    if (n-1 == 0) {
        std::cout << can[0] << std::endl;
        return 0;
    }
    can[n-2] = cant[n-2] = cant[n-1] + values[n-2];
    if (n-2 == 0) {
        std::cout << can[0] << std::endl;
        return 0;
    }
    cant[n-3] = max(cant[n-2], (ulong)max(values[n-3]+values[n-2], values[n-3]+values[n-1]));

    for (int i = n-4; i >= 0; i--) {
        cant[i] = max(
                max(cant[i+1], cant[i+2] + values[i]),
                cant[i+3] + values[i] + values[i+1]
        );
    }

    can[n-3] = can[n-2] + values[n-3];
    if (n-3 == 0) {
        std::cout << can[0] << std::endl;
        return 0;
    }

    uint val = values[n-4];
    for(uint i = n-3; i < n; i++) {
        if (values[i] < val) {
            val = values[i];
        }
    }
    can[n-4] = can[n-3] + values[n-4] - val;

    for (int i = n-5; i >= 0; i--) {
        can[i] = max(
                max(can[i+1], can[i+2] + values[i]),
                max(can[i+3] + values[i] + values[i+1], cant[i+4] + values[i] + values[i+1] + values[i+2])
        );
    }

    std::cout << can[0] << std::endl;
}