#include <bits/stdc++.h>
using namespace std;

/*
Understanding your BFS approach 

Problem in simple terms:
We have a grid (matrix) of 1s and 0s:
1 means land
0 means water
We need to count how many islands are there.
An island = connected group of 1s (connected up, down, left, right — not diagonally).

How your BFS works:
Think of it like painting the island:
Loop over each cell in the grid.
If you find a 1 and it hasn’t been visited → This is a new island.
From this cell, use BFS to visit all connected land cells and mark them as visited so they are not counted again.
Keep doing this until all land cells are visited.
BFS part (in your function breadthFirstSearch)
We use a queue to explore step-by-step.
Start with the first land cell → Push it in the queue.
Take one cell from the queue, visit its 4 neighbors (up, down, left, right).
If a neighbor is land and unvisited → Mark visited & push to queue.
Repeat until queue is empty → means the whole island is painted.

*/

void bfs(int sr, int sc, vector<vector<char>>& grid, vector<vector<int>>& vis) {
    int n = grid.size(), m = grid[0].size();
    queue<pair<int,int>> q;
    q.push({sr, sc});
    vis[sr][sc] = 1;

    vector<pair<int,int>> dirs = {{-1,0}, {1,0}, {0,-1}, {0,1}}; // up, down, left, right

    while (!q.empty()) {
        auto cell = q.front(); 
        q.pop();
        for (auto dir : dirs) {
            int r = cell.first, c = cell.second, dr = dir.first, dc = dir.second;
            int nr = r + dr, nc = c + dc;
            if (nr >= 0 && nr < n && nc >= 0 && nc < m 
                && grid[nr][nc] == '1' && !vis[nr][nc]) {
                vis[nr][nc] = 1;
                q.push({nr, nc});
            }
        }
    }
}

/*
Other approaches
(A) DFS — Recursive
Main function same logic as BFS, just call DFS instead of BFS.
*/
void dfs(int r, int c, vector<vector<char>>& grid, vector<vector<int>>& vis) {
    int n = grid.size(), m = grid[0].size();
    if (r < 0 || r >= n || c < 0 || c >= m || grid[r][c] != '1' || vis[r][c]) return;

    vis[r][c] = 1;
    dfs(r-1, c, grid, vis); // up
    dfs(r+1, c, grid, vis); // down
    dfs(r, c-1, grid, vis); // left
    dfs(r, c+1, grid, vis); // right
}

// DFS — Iterative (using stack)
void dfsIter(int sr, int sc, vector<vector<char>>& grid, vector<vector<int>>& vis) {
    int n = grid.size(), m = grid[0].size();
    stack<pair<int,int>> st;
    st.push({sr, sc});
    vis[sr][sc] = 1;

    vector<pair<int,int>> dirs = {{-1,0}, {1,0}, {0,-1}, {0,1}};

    while (!st.empty()) {
        auto cell = st.top(); st.pop();
        for (auto dir : dirs) {
            int r = cell.first, c = cell.second, dr = dir.first, dc = dir.second;
            int nr = r + dr, nc = c + dc;
            if (nr >= 0 && nr < n && nc >= 0 && nc < m 
                && grid[nr][nc] == '1' && !vis[nr][nc]) {
                vis[nr][nc] = 1;
                st.push({nr, nc});
            }
        }
    }
}

/*
(C) Union-Find / DSU
Idea: Treat each land cell as a node in a graph.
Initially, every land cell is its own island.
If two land cells are neighbors → union them.
Final island count = number of unique parents.
Pseudocode outline:
*/

class DSU {
public:
    vector<int> parent, size;
    DSU(int n) {
        parent.resize(n);
        size.resize(n, 1);
        iota(parent.begin(), parent.end(), 0);
    }
    int find(int x) {
        if (parent[x] != x) parent[x] = find(parent[x]);
        return parent[x];
    }
    void unite(int a, int b) {
        a = find(a); b = find(b);
        if (a != b) {
            if (size[a] < size[b]) swap(a, b);
            parent[b] = a;
            size[a] += size[b];
        }
    }
};

int numIslands(vector<vector<char>>& grid) {
    int n = grid.size(), m = grid[0].size();
    DSU dsu(n * m);
    int landCount = 0;
    vector<pair<int,int>> dirs = {{-1,0},{1,0},{0,-1},{0,1}};

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            if (grid[i][j] == '1') {
                landCount++;
                int id1 = i * m + j;
                for (auto dir : dirs) {
                    int ni = i + dir.first, nj = j + dir.second;
                    if (ni >= 0 && ni < n && nj >= 0 && nj < m && grid[ni][nj] == '1') {
                        int id2 = ni * m + nj;
                        if (dsu.find(id1) != dsu.find(id2)) {
                            dsu.unite(id1, id2);
                            landCount--;
                        }
                    }
                }
            }
        }
    }
    return landCount;
}
// This avoids explicit BFS/DFS.

/*
Time & Space Complexity
| Approach         | Time Complexity          | Space Complexity                   | Notes                                  |
| ---------------- | ------------------------ | ---------------------------------- | -------------------------------------- |
| BFS              | O(N×M)                   | O(N×M) (queue + visited)           | Visits each cell once                  |
| DFS (recursive)  | O(N×M)                   | O(N×M) (recursion stack + visited) | Risk of stack overflow for huge grids  |
| DFS (iterative)  | O(N×M)                   | O(N×M) (stack + visited)           | Safe from recursion depth limits       |
| Union-Find (DSU) | O(N×M × α(N×M)) ≈ O(N×M) | O(N×M)                             | α = inverse Ackermann, almost constant |
*/

int main() {
    vector<vector<char>> grid = {
        {'1','1','0','0','0'},
        {'1','1','0','0','0'},
        {'0','0','1','0','0'},
        {'0','0','0','1','1'}
    };

    int n = grid.size(), m = grid[0].size();
    vector<vector<int>> vis(n, vector<int>(m, 0));
    int count = 0;

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            if (grid[i][j] == '1' && !vis[i][j]) {
                count++;
                bfs(i, j, grid, vis);
            }
        }
    }

    cout << count << "\n";
    return 0;
}
