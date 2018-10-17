// Copyright 2018 Chen Zou. All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <algorithm>
#include <cstdlib>
#include <vector>

using std::min;
using std::max;
using std::vector;

typedef unsigned long long ull;
const ull MAX_COST = ((ull)1 << 62);
const int N = 51;
ull f[N][N];
ull dfs(const vector<int> &arr, int m[][N], int x, int y) {
  if (y - x == 2) {
    f[x][y] = arr[x] * arr[x + 1];
    return f[x][y];
  }
  if (y - x == 3) {
    f[x][y] = min(arr[x] * arr[x + 1] + arr[x + 2] * m[x][x + 2],
                  arr[x] * m[x + 1][x + 3] + arr[x + 1] * arr[x + 2]);
    return f[x][y];
  }
  if (f[x][y] < MAX_COST) return f[x][y];
  for (int j = x + 2; j < y - 1; j++) {
    f[x][y] = min(f[x][y], dfs(arr, m, x, j) + dfs(arr, m, j, y) +
                               m[x][j] * m[j][y]);
  }
  return f[x][y];
}

int calculateCost(const vector<int> &arr) {
  int n = arr.size();
  int m[N][N];
  for (int i = 0; i < n; i++)
    for (int j = 0; j <= n; j++) f[i][j] = MAX_COST;
  for (int i = 0; i < n; i++) {
    m[i][i + 1] = arr[i];
    for (int j = i + 2; j <= n; j++) m[i][j] = max(m[i][j - 1], arr[j - 1]);
  }

  return (int)dfs(arr, m, 0, n);
}

int main() {
  calculateCost(vector<int>({5, 4, 3, 2}));
  return 0;
}
