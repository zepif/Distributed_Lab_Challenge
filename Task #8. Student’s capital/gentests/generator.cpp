#include <bits/stdc++.h>

using namespace std;

int main() {
    random_device rd;
    mt19937 gen(rd());

    int k = gen() % 10 + 5;
    int capital = gen() % 100 + 1;
    int n = gen() % (k - 1) + 1;

    cout << k << " " << capital << " " << n << endl;
    
    int buy, sell;
    for (int i = 0; i < n; i++) {
        sell = gen() % 100 + 1;
        buy = gen() % sell + 1;
        cout << buy << " " << sell << endl;
    }
    
    for (int i = n; i < k - 1; i++)
        cout << gen() % 100 + 1 << " " << gen() % 100 + 1 << endl;
    cout << gen() % 100 + 1 << " " << gen() % 100 + 1;
    return 0;
}
