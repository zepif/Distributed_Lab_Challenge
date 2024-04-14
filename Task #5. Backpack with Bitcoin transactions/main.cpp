#include <bits/stdc++.h>
#include <fstream>
#include <chrono>
#include <cstring>

using namespace std;

struct Transaction {
    int id;
    int size;
    int fee;
};

bool cmp(const Transaction& a, const Transaction& b) {
    double ratioA = static_cast<double>(a.fee) / a.size;
    double ratioB = static_cast<double>(b.fee) / b.size;
    return ratioA > ratioB;
}

int knapsack(const vector<Transaction>& transactions, int maxSize) {
    int n = transactions.size();
    vector<int> dp(maxSize + 1, 0);

    for (int i = 0; i < n; i++) {
        for (int j = maxSize; j >= transactions[i].size; j--) {
            dp[j] = max(dp[j], dp[j - transactions[i].size] + transactions[i].fee);
        }
    }

    return dp[maxSize];
}

int main() {
    vector<Transaction> transactions;
    ifstream file("data.csv");
    if (file.is_open()) {
        string line;
        getline(file, line);
        while (getline(file, line)) {
            Transaction tx;
            char* token = strtok(const_cast<char*>(line.c_str()), ",");
            tx.id = atoi(token);

            token = strtok(nullptr, ",");
            tx.size = atoi(token);

            token = strtok(nullptr, ",");
            tx.fee = atoi(token);

            transactions.push_back(tx);
        }

        file.close();
    } else {
        cout << "Unable to open the file." << endl;
        return 1;
    }

    sort(transactions.begin(), transactions.end(), cmp);

    int maxSize = 1000000; // 1MB block size limit
    auto start = chrono::steady_clock::now();
    int totalFee = knapsack(transactions, maxSize);
    auto end = chrono::steady_clock::now();

    vector<Transaction> selectedTransactions;
    int currentSize = 0;
    for (const Transaction& tx : transactions) {
        if (currentSize + tx.size <= maxSize) {
            selectedTransactions.push_back(tx);
            currentSize += tx.size;
        } else {
            break;
        }
    }

    cout << "Constructed block:" << endl;
    for (const Transaction& tx : selectedTransactions) {
        cout << "Transaction ID: " << tx.id << ", Size: " << tx.size << " bytes, Fee: " << tx.fee << " satoshis" << endl;
    }
    cout << "Number of transactions in the block: " << selectedTransactions.size() << endl;
    cout << "Block size: " << currentSize << " bytes" << endl;
    cout << "Total extracted fee: " << totalFee << " satoshis" << endl;
    cout << "Construction time: " << chrono::duration_cast<chrono::microseconds>(end - start).count() << " microseconds" << endl;
    cout << "Max memory dedicated to storing intermediate pre-calculations: " << sizeof(int) * (maxSize + 1) << " bytes" << endl;

    return 0;
}
