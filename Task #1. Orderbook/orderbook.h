#ifndef ORDERBOOK_H
#define ORDERBOOK_H

#include <bits/stdc++.h>
using namespace std;

struct Order {
    int64_t user_id;
    int64_t amount;
    int64_t price;
    bool side; // true for buy, false for sell

    Order(int64_t user_id, int64_t amount, int64_t price, bool side);
};

struct BalanceChange {
    int64_t user_id;
    int64_t value;
    string currency;

    BalanceChange(int64_t user_id, int64_t value, const string& currency);
};

class OrderBook {
private:
    priority_queue<Order, vector<Order>, greater<Order>> sell_orders;
    priority_queue<Order, vector<Order>> buy_orders;
    unordered_map<int64_t, int64_t> uah_balances;
    unordered_map<int64_t, int64_t> usd_balances;

    void match_orders();
    void process_trade(int64_t seller_id, int64_t buyer_id, int64_t amount, int64_t total_price);

public:
    void add_order(const Order& order);
    void print_balances();
};

#endif // ORDERBOOK_H
