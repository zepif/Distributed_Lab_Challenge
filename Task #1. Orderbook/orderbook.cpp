#include "orderbook.h"

Order::Order(int64_t user_id, int64_t amount, int64_t price, bool side)
    : user_id(user_id), amount(amount), price(price), side(side) {}

BalanceChange::BalanceChange(int64_t user_id, int64_t value, const string& currency)
    : user_id(user_id), value(value), currency(currency) {}

bool operator<(const Order& lhs, const Order& rhs) {
    if (lhs.price != rhs.price) {
        return lhs.price < rhs.price;
    }
    return lhs.amount < rhs.amount;
}

bool operator>(const Order& lhs, const Order& rhs) {
    if (lhs.price != rhs.price) {
        return lhs.price > rhs.price;
    }
    return lhs.amount > rhs.amount;
}

void OrderBook::add_order(const Order& order) {
    if (order.side) {
        buy_orders.push(order);
    } else {
        sell_orders.push(order);
    }

    match_orders();
}

void OrderBook::match_orders() {
    while (!sell_orders.empty() && !buy_orders.empty()) {
        Order buy_order = buy_orders.top();
        Order sell_order = sell_orders.top();
        if (buy_order.price >= sell_order.price) {
            int64_t traded_amount = min(sell_order.amount, buy_order.amount);
            int64_t total_price = traded_amount * sell_order.price;

            process_trade(sell_order.user_id, buy_order.user_id, traded_amount, total_price);

            sell_order.amount -= traded_amount;
            buy_order.amount -= traded_amount;

            if (sell_order.amount == 0) {
                sell_orders.pop();
            }

            if (buy_order.amount == 0) {
                buy_orders.pop();
            }
        } else {
            break;
        }
    }
}

void OrderBook::process_trade(int64_t seller_id, int64_t buyer_id, int64_t amount, int64_t total_price) {
    uah_balances[seller_id] -= amount;
    usd_balances[seller_id] += total_price;

    uah_balances[buyer_id] += amount;
    usd_balances[buyer_id] -= total_price;

    cerr << "BalanceChange{" << seller_id << ", " << total_price << ", USD}" << endl;
    cerr << "BalanceChange{" << seller_id << ", " << -amount << ", UAH}" << endl;
    cerr << "BalanceChange{" << buyer_id << ", " << -total_price << ", USD}" << endl;
    cerr << "BalanceChange{" << buyer_id << ", " << amount << ", UAH}" << endl;
}

void OrderBook::print_balances() {
    cout << "Balances:" << endl;

    cout << "UAH Balances:" << endl;
    for (const auto& [user_id, balance] : uah_balances) {
        cout << "User " << user_id << " balance: " << balance << endl;
    }

    cout << "USD Balances:" << endl;
    for (const auto& [user_id, balance] : usd_balances) {
        cout << "User " << user_id << " balance: " << balance << endl;
    }

    
    cout << "Trades:" << endl;
    while (!sell_orders.empty() && !buy_orders.empty()) {
        Order sell_order = sell_orders.top();
        Order buy_order = buy_orders.top();

        if (buy_order.price >= sell_order.price) {
            int64_t traded_amount = min(sell_order.amount, buy_order.amount);
            int64_t total_price = traded_amount * sell_order.price;

            cout << "User " << sell_order.user_id << " sold " << traded_amount << " UAH at "
            << sell_order.price << " USD/UAH to User " << buy_order.user_id << endl;

            sell_order.amount -= traded_amount;
            buy_order.amount -= traded_amount;

            if (sell_order.amount == 0) {
                sell_orders.pop();
            }

            if (buy_order.amount == 0) {
                buy_orders.pop();
            }
        } else {
            //buy_orders.pop();
            break;
        }
    }

    while (!sell_orders.empty()) {
        Order order = sell_orders.top();
        cout << "User " << order.user_id << " has " << order.amount << " UAH available for sale at " 
        << order.price << " USD/UAH" << endl;
        sell_orders.pop();
    }

    while (!buy_orders.empty()) {
        Order order = buy_orders.top();
        cout << "User " << order.user_id << " has " << order.amount << " UAH available to buy at " 
        << order.price << " USD/UAH" << endl;
        buy_orders.pop();
    }

}
