#include <bits/stdc++.h>
using namespace std;

int ans = 0;

int f(vector <int> a, int count) {
    bool check = 1;
    for (int i = 0; i < a.size(); i++) {
        if (a[i]) {
            int t = a[i];
            a[i] = 0;
            int diff;
            diff = t * (i - 1 >= 0 ? a[i - 1] : 1) * (i + 1 < a.size() ? a[i + 1] : 1);
            f(a, count + diff);
            a[i] = t;
            check = 0;
        }
    }
    if (check) {
        ans = max(ans, count);
    }
}

int main() {
    int n;
    cin >> n;
    vector <int> a(n);
    for (auto &i : a)
        cin >> i;
    f(a, 0);
    cout << ans;
    return 0;
}

