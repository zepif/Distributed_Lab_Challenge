# Order Book

This is a C++ implementation of an order book component for trading UAH (base asset) and USD (quote asset). The order book matches buy and sell orders for UAH, allowing users to exchange UAH for USD.

### Structure

- `orderbook.h`: Contains the declarations for the `Order`, `BalanceChange`, and `OrderBook` classes.
- `orderbook.cpp`: Implements the functionality of the `OrderBook` class, including order management, order matching and balance updates. There were 2 priority queues implemented for this task. One stores offers for sale, the other stores offers for purchase
- `tests.cpp`: Contains a simple test case to demonstrate the usage of the `OrderBook` class.

### Usage

1. Clone the repository:

```bash
https://github.com/zepif/Distributed_Lab_Challenge.git

cd 'Task #1. Ordebook'
```

2. Build the project using CMake:

```bash
mkdir build

cd build

cmake ..

make
```

3. Run the compiled executable:

```bash
./orderbook_tests
```

### Time Spent

I spent about 2.5 hours solving this problem
