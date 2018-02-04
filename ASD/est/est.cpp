#include <iostream>
#include <algorithm>
#include <array>

using namespace std;

int n;
array<int, 100000> seq;

int main() {
    cin >> n;
    for(int i = 0; i < n; i++) {
        cin >> seq[i];
    }

    sort(begin(seq), begin(seq) + n);

    int counter = 1;
    int max = 1;
    for(int i = 1; i < n; i++)
        if(seq[i] - seq[i - 1] <= 1)
            counter++;
        else {
            max = std::max(max, counter);
            counter = 1;
        }

    cout << std::max(max, counter);
}