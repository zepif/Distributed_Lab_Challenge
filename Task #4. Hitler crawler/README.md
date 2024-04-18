# Task #4. Hitler Crawler

This is a console application that searches for the Wikipedia page on Adolf Hitler starting from a given Wikipedia page. The program uses asynchronous web scraping to efficiently traverse the Wikipedia link graph and find path to the Hitler page.

To optimise the program for multiple runs, was written a program that records the path from a random Wikipedia page to Hitler's page in a mini database. However, such paths are not always the shortest, but they speed up the programme a lot.

On average it takes about 20 seconds to find a path of length 3, the maximum was 100 seconds. Using the database, the average running time is less than 1 second.
Unfortunately I couldn't find a link to make the path longer than 3, so I can't say how long it will take to find Hitler's page.

### Structure

- `main.py` : Main script which finds way to the Hitler page. To find the path was used bfs. In our case, the
vertices of a directed graph are Wikipedia pages, and the edges are a jump to another page. In order to optimise the
entire search, asynchronous programming was applied to parsing pages to find other links and searching for links in the
database.

- `data.py` : A script that fills the database using main.py. This program passes a reference to the program input
and saves the path in paths.csv if it was found.

- `check_logs.py` : This programme compares logs in order to debug filtering of unnecessary references in main.py.

### Usage

Firstly, you need to clone the repository:

```bash
https://github.com/zepif/Distributed_Lab_Challenge.git

cd 'Task #4. Hitler crawler'
```

Then you need to install all necessary requirements:

```bash
pip install requirements.txt
```

To run the main script:

```bash
python main.py
```

To continue filling the database:

```bash
python data.py
```

### Time Spent

I spent about 2 days solving this problem

