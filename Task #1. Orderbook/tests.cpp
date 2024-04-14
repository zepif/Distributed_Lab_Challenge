#include "orderbook.cpp"

void run_tests() {
    OrderBook orderbook;
    orderbook.add_order(Order(1, 10, 25, false));
    orderbook.add_order(Order(5, 10, 35, false)); 
    orderbook.add_order(Order(2, 50, 30, true));
    orderbook.add_order(Order(3, 10, 32, true));
    orderbook.add_order(Order(7, 10, 35, true));
    orderbook.add_order(Order(4, 5, 3, true));
    orderbook.add_order(Order(6, 100, 15, false));
    //orderbook.print_balances();
}

int main() {
    run_tests();
    return 0;
}
