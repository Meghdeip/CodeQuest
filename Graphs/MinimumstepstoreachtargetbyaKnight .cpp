#include <bits/stdc++.h>
using namespace std;

void bfsKnight(vector<vector<int>>& chessboard, pair<int, int> knightPos, pair<int, int> targetPos) {
    queue<pair<int, int>> q;
    q.push(knightPos);
    chessboard[knightPos.first][knightPos.second] = 0;

    int row = chessboard.size(), col = chessboard[0].size();

    vector<pair<int, int>> dirs = {{2, 1}, {2, -1}, {-2, 1}, {-2, -1}, {1, 2}, {-1, 2}, {1, -2}, {-1, -2}};

    while (!q.empty()) {
        pair<int, int> cell = q.front();
        q.pop();

        for (auto dir : dirs) {
            int r = cell.first, c = cell.second, dr = dir.first, dc = dir.second;
            int nr = r + dr, nc = c + dc;

            if (nr == targetPos.first && nc == targetPos.second) {
                cout << chessboard[r][c] + 1 << '\n';
                return;
            }

            if (nr < row && nr >= 0 && nc < col && nc >= 0 && chessboard[nr][nc] == 0) {
                chessboard[nr][nc] = chessboard[r][c] + 1;
                q.push({nr, nc});
            }
        }
    }
}

int main() {

    int n;
    cin >> n;
    vector<vector<int>> chessboard(n, vector<int> (n, 0));

    pair<int, int> knightPos;
    cin >> knightPos.first >> knightPos.second;
    pair<int, int> targetPos;
    cin >> targetPos.first >> targetPos.second;

    bfsKnight(chessboard, knightPos, targetPos);

    // if (chessboard[targetPos.first][targetPos.second] != 0) cout << chessboard[targetPos.first][targetPos.second] << '\n';
    // else cout << 0 << '\n';

    return 0;
}
