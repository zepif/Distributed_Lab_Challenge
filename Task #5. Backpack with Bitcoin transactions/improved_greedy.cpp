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

int binarySearch(const vector<Transaction>& transactions, const vector<int>& prefixSum, int targetSize) {
    int left = 0;
    int right = transactions.size() - 1;
    int maxIndex = -1;

    while (left <= right) {
        int mid = left + (right - left) / 2;
        int currentSize = prefixSum[mid];

        if (currentSize <= targetSize) {
            maxIndex = mid;
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }

    return maxIndex;
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

    vector<int> prefixSum(transactions.size());
    prefixSum[0] = transactions[0].size;
    for (int i = 1; i < transactions.size(); i++) {
        prefixSum[i] = prefixSum[i - 1] + transactions[i].size;
    }

    int maxIndex = binarySearch(transactions, prefixSum, maxSize);
    auto end = chrono::steady_clock::now();

    int currentSize = 0;
    int totalFee = 0;
    vector<Transaction> selectedTransactions;

    for (int i = 0; i <= maxIndex; i++) {
        selectedTransactions.push_back(transactions[i]);
        currentSize += transactions[i].size;
        totalFee += transactions[i].fee;
    }

    cout << "Constructed block:" << endl;
    for (const Transaction& tx : selectedTransactions) {
        cout << "Transaction ID: " << tx.id << ", Size: " << tx.size << " bytes, Fee: " << tx.fee << " satoshis" << endl;
    }
    cout << "Number of transactions in the block: " << selectedTransactions.size() << endl;
    cout << "Block size: " << currentSize << " bytes" << endl;
    cout << "Total extracted fee: " << totalFee << " satoshis" << endl;
    cout << "Construction time: " << chrono::duration_cast<chrono::microseconds>(end - start).count() << " microseconds" << endl;
    cout << "Max memory dedicated to storing intermediate pre-calculations: " << sizeof(Transaction) * transactions.size() + sizeof(int) * transactions.size() << " bytes" << endl;

    return 0;
}
