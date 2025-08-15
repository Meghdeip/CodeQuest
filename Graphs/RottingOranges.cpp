#include <bits/stdc++.h>
using namespace std;

/*
Step-by-step explanation 

Think of the grid like a battlefield:
Fresh oranges (1) → healthy soldiers
Rotten oranges (2) → infected soldiers
Empty cells (0) → empty land

Goal:
Find the minimum minutes until all fresh soldiers are infected — or decide it’s impossible.

BFS Idea:
All rotten oranges start spreading together (multi-source BFS).
Every minute, the infection spreads to their neighbors (up, down, left, right).
BFS processes layer-by-layer → each layer = 1 minute.

Why BFS works best here:
Infection spreads equally in all directions.
BFS guarantees we count the minimum time for each orange to rot.
*/

// BFS Approach
int orangesRotting(vector<vector<int>>& grid) {
    int row = grid.size(), col = grid[0].size();
    queue<pair<int,int>> q;
    int fresh = 0;
        
    // Step 1: Push all initially rotten oranges into queue
    for (int i = 0; i < row; i++) {
    for (int j = 0; j < col; j++) {
        if (grid[i][j] == 2) q.push({i, j});
            else if (grid[i][j] == 1) fresh++;
        }
    }
        
    vector<pair<int,int>> dirs = {{-1,0},{1,0},{0,-1},{0,1}};
    int time = 0;

    // Step 2: BFS
        while (!q.empty() && fresh > 0) {
            int size = q.size(); // all rotten oranges for this minute
            for (int i = 0; i < size; i++) {
                pair<int,int> p = q.front();
                q.pop();

                for (int d = 0; d < dirs.size(); d++) {
                    int nx = p.first + dirs[d].first;
                    int ny = p.second + dirs[d].second;

                    // Check boundaries & if fresh
                    if (nx >= 0 && nx < row && ny >= 0 && ny < col && grid[nx][ny] == 1) {
                        grid[nx][ny] = 2; // rot it
                        fresh--;
                        q.push(make_pair(nx, ny));
                    }
                }
            }
            time++; // One minute passed
        }

    return fresh == 0 ? time : -1;
}

/*
DFS isn’t naturally time-layered like BFS, but we can make it work by passing time as a parameter and keeping track of the minimum time each cell gets infected.
*/

void dfs(vector<vector<int>>& grid, int x, int y, int time, vector<vector<int>>& minTime) {
    int row = grid.size(), col = grid[0].size();

    if (x < 0 || y < 0 || x >= row || y >= col) return;
    if (grid[x][y] == 0) return; // empty cell

    // If this cell is fresh and we are faster OR it's rotten and we're earlier
    if (time < minTime[x][y]) {
        minTime[x][y] = time;
        if (grid[x][y] == 1) grid[x][y] = 2; // rot it

        dfs(grid, x+1, y, time+1, minTime);
        dfs(grid, x-1, y, time+1, minTime);
        dfs(grid, x, y+1, time+1, minTime);
        dfs(grid, x, y-1, time+1, minTime);
    }
}

int orangesRotting(vector<vector<int>>& grid) {
    int row = grid.size(), col = grid[0].size();
    vector<vector<int>> minTime(row, vector<int>(col, INT_MAX));

    // Run DFS from every rotten orange
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            if (grid[i][j] == 2) {
                dfs(grid, i, j, 0, minTime);
            }
        }
    }

    int time = 0;
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            if (grid[i][j] == 1) return -1; // still fresh
            if (minTime[i][j] != INT_MAX)
                time = max(time, minTime[i][j]);
        }
    }
    return time;
}
/*
⚠️ DFS is less efficient here because it revisits cells a lot.
*/

struct State { int x, y, time; };
int orangesRotting(vector<vector<int>>& grid) {
    int row = grid.size(), col = grid[0].size();
    vector<vector<int>> minTime(row, vector<int>(col, INT_MAX));
    stack<State> st;

    // Push all rotten oranges
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            if (grid[i][j] == 2) {
                st.push({i, j, 0});
            }
        }
    }

    vector<pair<int,int>> dirs = {{-1,0},{1,0},{0,-1},{0,1}};

    while (!st.empty()) {
        State cur = st.top(); st.pop();
        int x = cur.x, y = cur.y, time = cur.time;

        if (x < 0 || y < 0 || x >= row || y >= col) continue;
        if (grid[x][y] == 0) continue;

        if (time < minTime[x][y]) {
            minTime[x][y] = time;
            if (grid[x][y] == 1) grid[x][y] = 2;
            for (int d = 0; d < 4; d++) {
                st.push({x + dirs[d].first, y + dirs[d].second, time + 1});
            }
        }
    }

    int ans = 0;
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            if (grid[i][j] == 1) return -1;
            if (minTime[i][j] != INT_MAX) ans = max(ans, minTime[i][j]);
        }
    }
    return ans;
}

/*
Union-Find Approach

This is unusual here, but possible if we model oranges as connected components.
Step 1: Treat each fresh/rotten orange as a node in DSU.
Step 2: Union rotten and fresh neighbors.
Step 3: For each rotten component, calculate the max distance to any fresh in it.
⚠️ This is not efficient for time tracking compared to BFS, but a good thought exercise.
*/

/*
Complexity Analysis

| Approach               | Time Complexity | Space Complexity | Pros                           | Cons                            |
| ---------------------- | --------------- | ---------------- | ------------------------------ | ------------------------------- |
| BFS                    | **O(m × n)**    | O(m × n)         | Fast, shortest time guaranteed | Needs queue                     |
| DFS Recursive          | O(m × n) worst  | O(m × n) stack   | Simple concept                 | Revisits cells, slower          |
| DFS Iterative          | O(m × n) worst  | O(m × n) stack   | No recursion limit             | Still revisits cells            |
| Brute Force Simulation | O(T × m × n)    | O(1)             | Easy to code                   | Very slow                       |
| Union-Find             | O(m × n α(mn))  | O(m × n)         | Interesting modeling           | Not practical for time problems |

*/

int main() {

    vector<vector<int>> grid = {
        {2,1,1},{0,1,1},{1,0,1}
    };

    int row = grid.size(), col = grid[0].size();
    vector<vector<int>> minute(row, vector<int> (col, 0));

    bool flag = true;
    int time = 0;

    int count = 0;
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            if (grid[i][j] == 1) {
                flag = false;
                count++;
            }
            if (minute[i][j] != 0) time = max(time, minute[i][j]);
        }
    }

    if (flag) cout << time << '\n';
    else if (count != 0) cout << -1 << '\n';
    else if (count == 0) cout << 0 << '\n';
    return 0;
}
