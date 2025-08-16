class Solution {
private:
    void bfs(vector<int> arr, int index, bool& flag) {
        queue<int> q;
        q.push(index);

        while (!q.empty()) {
            int n = q.front();
            q.pop();

            if (arr[n] == 0) {
                flag = true;
                return;
            }

            int jump = arr[n];
            arr[n] = -1;
            if (n - jump >= 0 && arr[n - jump] != -1)
                q.push(n - jump);
            if (n + jump < arr.size() && arr[n + jump] != -1)
                q.push(n + jump);
        }
    }

public:
    bool canReach(vector<int>& arr, int start) {
        bool flag = false;
        bfs(arr, start, flag);
        return flag;
    }
};
