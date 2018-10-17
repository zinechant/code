// Copyright 2018 Chen Zou. All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <vector>

using std::vector;
class Solution {
 public:
  int numIslands(const vector<vector<char>>& grid) {
    int m = grid.size();
    if (m == 0) {
      return 0;
    }
    int n = grid[0].size();

    int* p = new int[m * n * 2];
    int* q = new int[m * n * 2];

    const int z[4][2] = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}};

    bool* o = new bool[m * n * 2];
    for (int i = 0; i < m * n * 2; i++) {
      o[i] = false;
    }

    int ans = 0;

    for (int i = 0; i < m; i++) {
      for (int j = 0; j < n; j++) {
        if (grid[i][j] == '1' && !o[i * n + j]) {
          ans++;
          int h = 0;
          int t = 0;
          p[t] = i;
          q[t] = j;
          o[i * n + j] = true;
          while (h <= t) {
            for (int k = 0; k < 4; k++) {
              int x = p[h] + z[k][0];
              int y = q[h] + z[k][1];
              if (x < 0 || x >= m) continue;
              if (y < 0 || y >= n) continue;
              if (o[x * n + y]) continue;
              if (grid[x][y] == '0') continue;

              t++;
              p[t] = x;
              q[t] = y;
              o[x * n + y] = true;
            }
            h++;
          }
        }
      }
    }

    delete[] p;
    delete[] q;
    delete[] o;

    return ans;
  }
};
