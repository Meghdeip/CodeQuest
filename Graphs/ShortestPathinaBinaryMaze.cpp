/*
Goal:
You have a mat grid of 1s (walkable cells) and 0s (blocked cells).
Given a source cell (sx, sy) and a destination cell (dx, dy), we want to find the shortest number of steps to reach destination from source.

How BFS Works Here:
BFS explores in layers — meaning all cells at distance d are processed before any cell at distance d+1.
That’s why it’s perfect for finding the shortest path in an unweighted grid.

Steps in Your Code:
Create a queue to store cells (row, col) to visit.
Keep a dist matrix where dist[r][c] is the shortest distance from source to (r, c).
Initially, all distances are -1 (unvisited).

Start from the source cell:
dist[source] = 0
Push source into the queue.
While the queue is not empty:
Pop the front cell (BFS is FIFO).
Visit all 4 neighbors (Up, Down, Left, Right).
If neighbor is inside grid, not visited yet, and is walkable (mat[r][c] == 1):
Update its distance.
Push it into the queue.

Why BFS works for shortest path here:
Because BFS always processes the closest cells first, the first time you reach a cell, you’ve already found the shortest path to it.
*/

#include <bits/stdc++.h>
using namespace std;

void bfs(vector<vector<int>>& mat, pair<int,int> src, pair<int,int> dest, vector<vector<int>>& dist) {
    int row = mat.size(), col = mat[0].size();
    queue<pair<int,int>> q;
    q.push(src);
    dist[src.first][src.second] = 0;

    vector<pair<int,int>> dirs = {{-1,0},{1,0},{0,-1},{0,1}}; // Up, Down, Left, Right

    while (!q.empty()) {
        pair<int, int> cell = q.front(); 
        q.pop();

        for (auto  dir: dirs) {
            int nr = cell.first + dir.first, nc = cell.second + dir.second;
            if (nr >= 0 && nr < row && nc >= 0 && nc < col &&
                dist[nr][nc] == -1 && mat[nr][nc] == 1) {
                dist[nr][nc] = dist[cell.first][cell.second] + 1;
                q.push({nr, nc});
            }
        }
    }
}

/*
Other Possible Approaches
(a) DFS Recursive
DFS will explore one path fully before backtracking.
Problem: DFS cannot guarantee shortest path in an unweighted graph unless you explore all paths and take the minimum, which is slower.
Still, you can implement DFS to just check reachability.
*/

bool dfsRec(vector<vector<int>>& mat, vector<vector<bool>>& vis, int r, int c, int dr, int dc) {
    if (r == dr && c == dc) return true;
    vis[r][c] = true;

    vector<pair<int,int>> dirs = {{-1,0},{1,0},{0,-1},{0,1}};
    for (auto dir : dirs) {
        int nr = r + dir.first, nc = c + dir.second;
        if (nr >= 0 && nr < mat.size() && nc >= 0 && nc < mat[0].size() &&
            !vis[nr][nc] && mat[nr][nc] == 1) {
            if (dfsRec(mat, vis, nr, nc, dr, dc)) return true;
        }
    }
    return false;
}

/*
Complexity:
Time: O(R * C) visiting each cell once.
Space: O(R * C) recursion stack + visited array.
*/

/*
(b) DFS Iterative (Stack)
Same logic as recursive, but using an explicit stack.
Still doesn’t guarantee shortest path without extra logic.
*/

bool dfsIter(vector<vector<int>>& mat, pair<int,int> src, pair<int,int> dest) {
    int row = mat.size(), col = mat[0].size();
    vector<vector<bool>> vis(row, vector<bool>(col, false));
    stack<pair<int,int>> st;
    st.push(src);

    vector<pair<int,int>> dirs = {{-1,0},{1,0},{0,-1},{0,1}};

    while (!st.empty()) {
        auto cell = st.top(); 
        st.pop();
        int r = cell.first, c = cell.second;
        if (r == dest.first && c == dest.second) return true;
        if (vis[r][c]) continue;
        vis[r][c] = true;

        for (auto dir : dirs) {
            int nr = r + dir.first, nc = c + dir.second;
            if (nr >= 0 && nr < row && nc >= 0 && nc < col &&
                !vis[nr][nc] && mat[nr][nc] == 1) {
                st.push({nr, nc});
            }
        }
    }
    return false;
}

/*
(c) Union-Find (Disjoint Set Union - DSU)
Idea: Treat each cell as a node. If two cells are adjacent and walkable, union them.
Then check if source and destination belong to the same connected component.
Good for: Multiple connectivity queries after preprocessing.
Bad for: Shortest path — DSU only checks connectivity, not distance.
*/

/*
Time & Space Complexity Summary
| Approach      | Shortest Path?          | Time Complexity   | Space Complexity                 |
| ------------- | ----------------------- | ----------------- | -------------------------------- |
| BFS           | ✅ Yes                   | `O(R*C)`          | `O(R*C)` for dist + queue        |
| DFS Recursive | ❌ Not guaranteed        | `O(R*C)`          | `O(R*C)` for visited + recursion |
| DFS Iterative | ❌ Not guaranteed        | `O(R*C)`          | `O(R*C)` for visited + stack     |
| Union-Find    | ❌ Not for shortest path | `O(R*C * α(R*C))` | `O(R*C)`                         |
*/

int main() {

    vector<vector<int>> mat = {
        {1, 1, 1, 1, 1}, 
        {1, 1, 1, 1, 1},
        {1, 1, 1, 1, 0}, 
        {1, 0, 1, 0, 1}
    };

    int row = mat.size(), col = mat[0].size();

    pair<int, int> source;
    pair<int, int> destination;

    vector<vector<int>> dist(row, vector<int> (col, -1));;

    cin >> source.first >> source.second;
    cin >> destination.first >> destination.second;

    bfs(mat, source, destination, dist);

    cout << dist[destination.first][destination.second];

    return 0;
}
