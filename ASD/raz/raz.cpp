#include <iostream>
#include <set>

#define uint unsigned int
#define MaxLength 500000

#define Yes "TAK"
#define No "NIE"

uint input[MaxLength];

bool check(std::set<int> const& ones, std::set<int> const& twos, std::set<int> const& threes) {
    if (ones.empty() || twos.empty() || threes.empty())
        return false;
    auto iter1 = ones.upper_bound(0);
    auto iter3 = threes.lower_bound(MaxLength+1);
    if (iter1 == ones.end() || iter3 == threes.begin()) {
        return false;
    } else {
        iter3--;
        auto iter2 = twos.upper_bound(*iter1);
        return iter2 != twos.end() && (*iter2) <= (*iter3);
    }
}

int main() {
    int n;
    std::cin >> n;

    std::set<int> sets[3];
    for (uint i = 0; i < n; i++) {
        std::cin >> input[i];
        sets[input[i]-1].insert(i+1);
    }

    if(check(sets[0], sets[1], sets[2])) {
        std::cout << Yes << std::endl;
    } else {
        std::cout << No << std::endl;
    }

    int m;
    std::cin >> m;
    for (uint j = 0; j < m; j++) {
        uint i, x;
        std::cin >> i >> x;
        int was = input[i-1];
        if (x != was) {
            sets[was-1].erase(i);
            sets[x-1].insert(i);
            input[i-1] = x;
        }

        if(check(sets[0], sets[1], sets[2])) {
            std::cout << Yes << std::endl;
        } else {
            std::cout << No << std::endl;
        }
    }
}

