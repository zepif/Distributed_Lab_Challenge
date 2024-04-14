# Task #5. Backpack with Bitcoin transactions

This is a problem, as it is clear from the problem condition ;) on the knapsack algorithm and an attempt to somehow
optimise it in terms of time at the cost of accuracy of the solution. Since the amount of data is 10^5 and the running
time is O(n^2), it is obvious that the knapsack algorithm will run for a very long time.
Perhaps potential solutions should be sought in non-standard approaches: ant algorithm, genetic algorithm, neural networks.
Probably there may be some non-standard approach in boost c++ library for solving the knapsack problem as well as for other similar problems. However, I have not found such an approach

### Structure

- `gen.py` : Generates data for solutions

- `main.cpp` : The standard knapsack algorithm is implemented

- `greedy.cpp` : Simple greedy algorithm

- `improved_greedy.cpp` : A bit improved greedy algorithm :)

### Usage

Firstly, you need to clone the repository:

```bash
https://github.com/zepif/Distributed_Lab_Challenge.git

cd 'Task #5. Backpack with Bitcoin transactions'
```

Use ```bash g++ -o sol ...``` to compile one of the solutions.

Or

```bash python3 gen.py``` to generate the data

### Time Spent

I spent about 3 hours solving this problem



