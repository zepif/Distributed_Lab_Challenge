# Task #3. Website Analytics

### Time complexity

Let the number of rows in day1.csv be N, and the number of rows in day2.csv be M. 

Then finding people who have visited some pages is O(n + m) in time and O(n + m) in memory, since I use the hash set
structure for this.

For finding people who visited new products on the second day it works for O(n + m) in time and O(n + m) in memory. Since iterrows loops and adding products to sets have O(n) asymptotics in time. Subtracting sets and checking emptiness have O(n + m) asymptotics in time. 
Thus, all in all complexity is O(n + m) in time.

### Structure

- `gen.py` : Generates test data on settings from `config.yaml`. In config several settings: number of users;
number of products; size of day 1 & day 2 data.

- `main.py` : Main script that solves the problem as written in the paragraph above.

### Usage

Firstly, you need to clone the repository:

```bash
https://github.com/zepif/Distributed_Lab_Challenge.git

cd 'Task #3. Website analytics'
```

Then you need to install all necessary requirements:

```bash
pip install requirements.txt
```

To run the main script:

```bash
python main.py
```

To generate new data:

```bash
python gen.py
```

