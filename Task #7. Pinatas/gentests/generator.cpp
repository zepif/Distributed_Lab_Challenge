#include <bits/stdc++.h>
using namespace std;

int main() {
    random_device rd;
    mt19937 gen(rd());

    int n;
    n = gen() % 10 + 1;
    cout << n << endl;
    for (int i = 0; i < n - 1; i++) {
        cout << gen() % 100 << " ";
    }
    cout << gen() % 100;
    return 0;
}
