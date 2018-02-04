#include <iostream>
#include <set>

#define uint unsigned int
#define ulong unsigned long long

int main() {
    uint n;
    std::cin >> n;

    std::set<uint> numbers;
    ulong result = 0;

    uint num;
    std::cin >> num;
    numbers.insert(num);

    for (int i = 1; i < n; i++) {
        std::cin >> num;

        auto iter = numbers.upper_bound(num);
        if (iter == numbers.begin()) {
            // will insert at the beginning
            uint gr = *iter;
            long long diff = (gr - num);
            diff *= diff;
            result += diff;
        }
        else if (iter == numbers.end()) {
            // will insert at the end
            iter--;
            uint less = *iter;
            long long diff = (num - less);
            diff *= diff;
            result += diff;
        }
        else {
            // will insert at the end
            uint gr = *iter;
            iter--;
            uint less = *iter;

            long long diff1 = gr - num;
            diff1 *= diff1;
            long long diff2 = num - less;
            diff2 *= diff2;
            long long diff = (gr - less);
            diff *= diff;
            diff = -diff;
            diff += diff1 + diff2;
            result += diff;
        }
        numbers.insert(num);
        std::cout << result << std::endl;
    }
}