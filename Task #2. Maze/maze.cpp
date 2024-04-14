#include <bits/stdc++.h>

using namespace std;

enum CellType {
    UNVISITED = 0,
    ROAD = 1,
    WALL = 2,
    ENTRANCE = 3,
    EXIT = 4,
    TREASURE = 5,
    TRAP = 6
};

static const int dx[4] = {0, 0, 1, -1};
static const int dy[4] = {1, -1, 0, 0};

int bfs(const vector<vector<int>>& maze, int startRow, int startCol, int endRow, int endCol) {
    int rows = maze.size();
    int cols = maze[0].size();
    vector<vector<bool>> visited(rows, vector<bool>(cols, false));
    queue<pair<int, int>> q;
    q.push({startRow, startCol});
    visited[startRow][startCol] = true;
    int steps = 0;
    while (!q.empty()) {
        int size = q.size();
        for (int i = 0; i < size; i++) {
            int row = q.front().first;
            int col = q.front().second;
            q.pop();
            if (row == endRow && col == endCol) {
                return steps;
            }
            for (int j = 0; j < 4; j++) {
                int newRow = row + dx[j];
                int newCol = col + dy[j];
                if (newRow >= 0 && newRow < rows && newCol >= 0 && newCol < cols && maze[newRow][newCol] != WALL && !visited[newRow][newCol]) {
                    if (maze[newRow][newCol] == TRAP) {
                        steps++;
                    }
                    q.push({newRow, newCol});
                    visited[newRow][newCol] = true;
                }
            }
        }
        steps++;
    }
    return 1488;
}

void place_treasure(vector<vector<int>>& maze, int rows, int cols) {
    int treasure_chance = 32;
    if (rand() % 100 > treasure_chance)
        return;
    bool treasure_placed = false;
    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < cols; col++) {
            if (maze[row][col] == ROAD) {
                bool on_path = false;
                for (int i = 0; i < rows; i++) {
                    for (int j = 0; j < cols; j++) {
                        if (maze[i][j] == ENTRANCE) {
                            if (bfs(maze, i, j, row, col) <= 3) {
                                maze[row][col] = TREASURE;
                                treasure_placed = true;
                            }
                            break;
                        }
                    }
                    if (treasure_placed) break;
                }
            }
        }
    }
}

void place_traps(vector<vector<int>>& maze, int rows, int cols) {
    int num_traps = rand() % 6;
    vector<pair<int, int>> traps;
    for (int i = 0; i < num_traps; i++) {
        bool found_trap = false;
        while (!found_trap) {
            int row = rand() % rows;
            int col = rand() % cols;
            if (maze[row][col] == ROAD) {
                maze[row][col] = TRAP;
                traps.emplace_back(row, col);
                found_trap = true;
            }
        }
    }
}

void set_entrance_exit(vector<vector<int>>& maze, int rows, int cols) {
    vector<pair<int, int>> entrances, exits;
    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < cols; col++) {
            if (maze[row][col] == ROAD) {
                if (row == 1 || row == rows - 2 || col == 1 || col == cols - 2) {
                    if (row == 1 || col == 1) {
                        entrances.emplace_back(row, col);
                    } else {
                        exits.emplace_back(row, col);
                    }
                }
            }
        }
    }

    auto entrance = entrances[rand() % entrances.size()];
    auto exit = exits[rand() % exits.size()];
    while (entrance == exit) {
        exit = exits[rand() % exits.size()];
    }
    maze[entrance.first][entrance.second] = ENTRANCE;
    maze[exit.first][exit.second] = EXIT;
}

void generate_maze(vector<vector<int>>& maze, int row, int col) {
    maze[row][col] = ROAD;
    vector<int> directions(4);
    iota(directions.begin(), directions.end(), 0);
    srand(time(NULL) + row * col + col);
    random_shuffle(directions.begin(), directions.end());
    for (int dir : directions) {
        int new_row = row + 2 * dx[dir];
        int new_col = col + 2 * dy[dir];
        if (new_row >= 0 && new_row < maze.size() && new_col >= 0 && new_col < maze[0].size() && maze[new_row][new_col] == UNVISITED) {
            maze[row + dx[dir]][col + dy[dir]] = ROAD;
            maze[new_row][new_col] = ROAD;
            generate_maze(maze, new_row, new_col);
        }
    }
}

int main() {
    int rows, cols;
    cout << "Enter the number of rows:\n";
    cin >> rows;
    cout << "Enter the number of columns:\n";
    cin >> cols;

    vector<vector<int>> maze(rows, vector<int>(cols, UNVISITED));
    generate_maze(maze, 0, 0);
    set_entrance_exit(maze, rows, cols);
    place_traps(maze, rows, cols);
    place_treasure(maze, rows, cols);
    
    for (const auto& row : maze) {
        for (int cell : row) {
            switch (cell) {
            case UNVISITED:
                cout << "\033[1;37m* \033[0m";
                break;
            case ROAD:
                cout << "\033[1;32m. \033[0m";
                break;
            case WALL:
                cout << "\033[1;37m* \033[0m";
                break;
            case ENTRANCE:
                cout << "\033[1;33mE \033[0m";
                break;
            case EXIT:
                cout << "\033[1;33mX \033[0m";
                break;
            case TREASURE:
                cout << "\033[1;34m^ \033[0m";
                break;
            case TRAP:
                cout << "\033[1;31mT \033[0m";
                break;
            }
        }
        cout << endl;
    }
    
    return 0;
}
