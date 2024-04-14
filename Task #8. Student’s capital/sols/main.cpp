#include <bits/stdc++.h>
using namespace std;

int main() {
    int k, capital, n;
    cin >> k >> capital >> n;
    vector <pair<int, int>> a;
    for (int i = 0; i < k; i++) {
        int x, y;
        cin >> x >> y;          // first buy, second sell
        if (x <= y)
            a.push_back({x, y});
    } 
    sort(a.begin(), a.end());
    priority_queue<int> q;
    int count = 0;
    k = a.size();
    for (int i = 0; i < k && count < n; i++) {
        if (capital >= a[i].first) {
            q.push({a[i].second - a[i].first});
            continue;
        }
        while (count < n && capital < a[i].first && !q.empty()) {
            if (q.top() <= 0)
                break;
            cerr << "top : " << q.top() << endl;
            capital += q.top();
            cerr << "current C : " << capital << endl;
            q.pop();
            count++;
        }
    }
    while (count < n && !q.empty()) {
        if (q.top() <= 0)
            break;
        cerr << "top : " << q.top() << endl;
        capital += q.top();
        cerr << "current C : " << capital << endl;
        q.pop();
        count++;
    }
    cout << capital;
    return 0;
}

