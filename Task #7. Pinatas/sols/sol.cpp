#include <bits/stdc++.h>
using namespace std;
int main() {
    int n;
    cin >> n;
    vector <int> a(n);
    for (auto &i : a)
        cin >> i;
    int dp[n][2] = {};
    dp[0][0] = 0;
    dp[0][1] = a[0] * (n >= 2 ? a[1] : 1);
    for (int i = 1; i < n; i++) {
        dp[i][0] = max(dp[i - 1][0], dp[i - 1][1]);
        dp[i][1] = dp[i - 1][0] + a[i - 1] * a[i] * (i + 1 < n ? a[i + 1] : 1);
    }
    cout << max(dp[n - 1][0], dp[n - 1][1]);
    return 0;
}

