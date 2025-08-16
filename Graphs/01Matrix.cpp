class Solution {
private:
    void bfs(vector<vector<int>>& mat, vector<vector<int>>& dist) {
        queue<pair<int, int>> q;
        int row = mat.size(), col = mat[0].size();

        for (int i = 0; i < row; i++) {
            for (int j = 0; j < col; j++) {
                if (mat[i][j] == 0) {
                    q.push({i, j});
                }
            }
        }

        vector<pair<int, int>> dirs = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};

        while (!q.empty()) {
            pair<int, int> p = q.front();
            q.pop();

            for (auto dir : dirs) {
                int r = p.first, c = p.second, dr = dir.first, dc = dir.second;
                int nr = r + dr, nc = c + dc;

                if (nr >= 0 && nr < mat.size() && nc >= 0 &&
                    nc < mat[0].size() && mat[nr][nc] == 1) {
                    if (dist[nr][nc] != 0)
                        dist[nr][nc] = min(dist[nr][nc], dist[r][c] + 1);
                    else
                        dist[nr][nc] = dist[r][c] + 1;
                    q.push({nr, nc});
                    mat[nr][nc] = 0;
                }
            }
        }
    }

public:
    vector<vector<int>> updateMatrix(vector<vector<int>>& mat) {
        int row = mat.size(), col = mat[0].size();
        vector<vector<int>> dist(row, vector<int>(col, 0));
        bfs(mat, dist);
        return dist;
    }
};
