// Optimized BFS Approach 

/*
1️⃣ Beginner-Friendly Explanation of Your BFS Flood Fill
Think of image as a grid of pixels, where each cell has a color number.
We start at (sr, sc) and want to change it to color. The BFS (Breadth-First Search) approach works like painting outward in waves:

Step-by-step
Step 1: Store the original color of the starting pixel in prevColor.
This helps us know which pixels are allowed to be painted.
Step 2: Push (sr, sc) into a queue and immediately recolor it.
Step 3: While the queue is not empty:
Take the first pixel (r, c) from the queue.
Look in all 4 directions (up, down, left, right).
If the neighbor:
Is inside the grid
Has the same original color (prevColor)
Is not already the new color (color)
→ Then recolor it and push it into the queue for processing later.
Step 4: Keep going until the queue is empty, meaning no more same-colored neighbors to paint.

2️⃣ Suggested Improvements to Your BFS
Readability Improvements
Use a directions array to avoid repeating up/down/left/right code.
Remove redundant boundary checks like p.first + 1 >= 0 — BFS never produces negative indexes if directions are correct.
Remove repeated color != prevColor checks inside BFS — you can just check once at the start.

*/
#include <bits/stdc++.h>
using namespace std;

void bfsFloodFill(int sr, int sc, vector<vector<int>>& image, int newColor) {
    int prevColor = image[sr][sc];
    if (prevColor == newColor) return; // no need to do anything

    int rows = image.size();
    int cols = image[0].size();

    queue<pair<int, int>> q;
    q.push({sr, sc});
    image[sr][sc] = newColor;

    // directions: up, down, left, right
    vector<pair<int, int>> dirs = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

    while (!q.empty()) {
        pair<int, int> cell = q.front();
        q.pop();
        int r = cell.first;
        int c = cell.second;

        for (int i = 0; i < dirs.size(); i++) {
            int dr = dirs[i].first;
            int dc = dirs[i].second;

            int nr = r + dr;
            int nc = c + dc;

            // Check bounds and whether the color matches
            if (nr >= 0 && nr < rows && nc >= 0 && nc < cols && image[nr][nc] == prevColor) {
                image[nr][nc] = newColor; // Recolor the cell
                q.push({nr, nc});        // Push into the BFS queue
            }
        }
    }

}

int main() {
    vector<vector<int>> image = {
        {1,1,1},
        {1,1,0},
        {1,0,1}
    };

    int sr, sc, color;
    cin >> sr >> sc >> color;

    bfsFloodFill(sr, sc, image, color);

    for (auto &row : image) {
        for (auto &val : row) cout << val << ' ';
        cout << '\n';
    }

    return 0;
}

/*
Beginner-Friendly Explanation

Think of image like a 2D coloring book — each cell has a color.
We start at position (sr, sc) (starting row and column), and we want to change its color to newColor. But not just that one — we also want to color all connected neighbors that have the same starting color, just like a "paint bucket" tool in MS Paint.
Here’s how DFS works for this:
Check the pixel’s color
If it matches the original starting color, we can change it to the new color.
Paint it
Change the current pixel to newColor.
Explore neighbors
Look in 4 directions: up, left, down, right.
For each neighbor:
If it’s inside the grid and has the original color, repeat the process (recursive call).
Stop conditions
If the pixel is outside the image boundaries → stop.
If the pixel’s color isn’t the original color → stop.
So, DFS basically says:
“Paint this pixel, then go paint all same-colored neighbors, and for each of them, paint their same-colored neighbors, and so on, until no more pixels can be painted.”

Complexity
Time Complexity: O(N) where N = number of pixels in the image (rows × cols).
Each pixel is painted at most once.
Space Complexity: O(N) in the worst case because recursion uses the call stack.

Alternate Approaches
BFS (Queue-based)
Instead of recursion, use a queue to store pixels to paint.
This avoids deep recursion issues (stack overflow) for large images.
Works by processing one pixel at a time and adding its same-colored neighbors to the queue.

Iterative DFS (using stack)
Similar to BFS but with a stack instead of a queue.
Avoids recursion and gives the same result as DFS.
In-place scan without explicit visited array
Since we are changing the color directly in the image, no need for a separate visited array — visited cells are recognized by having newColor.

Connected Component Labeling (Union-Find)
Overkill for flood fill, but possible:
First, find all connected cells with the original color using Union-Find (Disjoint Set Union), then recolor them.
Useful if you want to do many flood fills on the same image efficiently.
*/

// DFS Recursive Approach
void dfsFloodFill(vector<vector<int>>& image, int r, int c, int prevColor, int newColor) {
    if (r < 0 || r >= image.size() || c < 0 || c >= image[0].size()) return;
    if (image[r][c] != prevColor) return;

    image[r][c] = newColor;

    dfsFloodFill(image, r-1, c, prevColor, newColor); // up
    dfsFloodFill(image, r+1, c, prevColor, newColor); // down
    dfsFloodFill(image, r, c-1, prevColor, newColor); // left
    dfsFloodFill(image, r, c+1, prevColor, newColor); // right
}

// DFS iterative Stack Approach
void dfsIterativeFloodFill(vector<vector<int>>& image, int sr, int sc, int newColor) {
    int prevColor = image[sr][sc];
    if (prevColor == newColor) return;

    stack<pair<int,int>> st;
    st.push({sr, sc});

    vector<pair<int,int>> dirs = {{-1,0}, {1,0}, {0,-1}, {0,1}};

    while (!st.empty()) {
        pair<int, int> cell = st.top();
        st.pop();
        int r = cell.first;
        int c = cell.second;

        if (r < 0 || r >= image.size() || c < 0 || c >= image[0].size()) continue;
        if (image[r][c] != prevColor) continue;

        image[r][c] = newColor;

        for (size_t i = 0; i < dirs.size(); i++) {
            int dr = dirs[i].first;
            int dc = dirs[i].second;
            st.push({r + dr, c + dc});
        }
    }
}

// Union-Find Approach (Overkill, for many flood fills)
struct DSU {
    vector<int> parent, size;
    DSU(int n) : parent(n), size(n, 1) { iota(parent.begin(), parent.end(), 0); }
    int find(int x) { return parent[x] == x ? x : parent[x] = find(parent[x]); }
    void unite(int a, int b) {
        a = find(a), b = find(b);
        if (a != b) {
            if (size[a] < size[b]) swap(a, b);
            parent[b] = a;
            size[a] += size[b];
        }
    }
};

void unionFindFloodFill(vector<vector<int>>& image, int sr, int sc, int newColor) {
    int prevColor = image[sr][sc];
    if (prevColor == newColor) return;
    int rows = image.size(), cols = image[0].size();
    DSU dsu(rows * cols);

    auto idx = [&](int r, int c) { return r * cols + c; };
    vector<pair<int,int>> dirs = {{-1,0}, {1,0}, {0,-1}, {0,1}};

    // Step 1: Union all connected same-color cells
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            if (image[r][c] == prevColor) {
                for (size_t k = 0; k < dirs.size(); k++) {
                    int dr = dirs[k].first;
                    int dc = dirs[k].second;
                    int nr = r + dr;
                    int nc = c + dc;

                    if (nr >= 0 && nr < rows && nc >= 0 && nc < cols &&
                        image[nr][nc] == prevColor) {
                        dsu.unite(idx(r, c), idx(nr, nc));
                    }
                }
            }
        }
    }

    // Step 2: Recolor all cells in the same component as start
    int comp = dsu.find(idx(sr, sc));
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            if (dsu.find(idx(r, c)) == comp) {
                image[r][c] = newColor;
            }
        }
    }
}

/* This was my appraoch
#include <bits/stdc++.h>
using namespace std;

void breadthFirstSearch(int sr, int sc, vector<vector<int>>& image, int color) {
    queue<pair<int, int>> q;
    q.push(make_pair(sr, sc));
    int prevColor = image[sr][sc];
    image[sr][sc] = color;

    while (!q.empty()) {
        pair<int, int> p = q.front();
        q.pop();

        // Down
        if (p.first + 1 >= 0 && p.second >= 0 && p.first + 1 < image.size() && p.second < image[0].size()) {
            if (image[p.first + 1][p.second] == prevColor && color != prevColor) {
                image[p.first + 1][p.second] = color;
                pair<int, int> np = make_pair(p.first + 1, p.second);
                q.push(np);
            }
        }

        // Up
        if (p.first - 1 >= 0 && p.second >= 0 && p.first - 1 < image.size() && p.second < image[0].size()) {
            if (image[p.first - 1][p.second] == prevColor && color != prevColor) {
                image[p.first - 1][p.second] = color;
                pair<int, int> np = make_pair(p.first - 1, p.second);
                q.push(np);
            }
        }

        // Left
        if (p.first >= 0 && p.second - 1 >= 0 && p.first < image.size() && p.second - 1 < image[0].size()) {
            if (image[p.first][p.second - 1] == prevColor && color != prevColor) {
                image[p.first][p.second - 1] = color;
                pair<int, int> np = make_pair(p.first, p.second - 1);
                q.push(np);
            }
        }

        // Right
        if (p.first >= 0 && p.second + 1 >= 0 && p.first < image.size() && p.second + 1 < image[0].size()) {
            if (image[p.first][p.second + 1] == prevColor && color != prevColor) {
                image[p.first][p.second + 1] = color;
                pair<int, int> np = make_pair(p.first, p.second + 1);
                q.push(np);
            }
        }
    }
}

int main() {

    vector<vector<int>> image = {
        {1,1,1},
        {1,1,0},
        {1,0,1}
    };

    int sr, sc, color;
    cin >> sr >> sc >> color;

    int row = image.size(), col = image[0].size();
    breadthFirstSearch(sr, sc, image, color);
    // vector<vector<int>> visited(row, vector<int> (col, 1));
    // for (int i = 0; i < row; i++) {
    //     for (int j = 0; j < col; j++) {
    //         if (visited[i][j])
    //     }
    // }

    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            cout << image[i][j] << ' ';
        }
        cout << '\n';
    }


    return 0;
}
*/
