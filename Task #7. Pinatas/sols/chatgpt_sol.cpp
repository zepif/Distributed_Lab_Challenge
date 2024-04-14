#include <bits/stdc++.h>
using namespace std;

int maxCandies(vector<int>& pinatas) {
    int n = pinatas.size();
    if (n == 0) {
        return 0;
    }

    vector<int> dp(n, 0);

    dp[0] = pinatas[0];
    dp[1] = max(pinatas[0], pinatas[1]);

    for (int i = 2; i < n; i++) {
        int candies = pinatas[i-1] * pinatas[i] * ((i+1 < n) ? pinatas[i+1] : 1);
        dp[i] = max(dp[i-1], (i-2 >= 0 ? dp[i-2] : 0) + candies);
    }

    return dp[n-1];
}

int main() {
    int n;
    cin >> n;
    vector <int> a(n);
    for (auto &i : a)
        cin >> i;
    cout << maxCandies(a);
    return 0;
}
