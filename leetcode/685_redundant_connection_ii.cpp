// Copyright 2018 Chen Zou. All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <queue>
#include <vector>

using std::vector;

class Solution {
 public:
  vector<int> findRedundantDirectedConnection(
      const vector<vector<int> > &edges) {
    const int N = 1000;
    int n = edges.size();
    vector<std::pair<int, int> > a[N];

    bool f[N];
    for (int i = 0; i < N; i++) f[i] = true;

    std::queue<int> o;

    int d[N];
    for (int i = 0; i < N; i++) d[i] = 0;

    int i = 0;
    for (const auto &edge : edges) {
      const int p = edge[0] - 1;
      const int q = edge[1] - 1;
      a[p].push_back(std::pair<int, int>(q, i));
      d[q]++;
      i++;
    }

    for (int i = 0; i < n; i++) {
      if (d[i] == 0) {
        o.push(i);
      }
    }

    while (!o.empty()) {
      int x = o.front();
      o.pop();
      for (const auto &edge : a[x]) {
        int y = edge.first;
        int k = edge.second;
        f[k] = false;
        d[y]--;
        if (!d[y]) o.push(y);
      }
    }

    for (int i = n - 1; i >= 0; i--) {
      if (f[i]) return edges[i];
    }
  }
};

int main() {
  Solution solution;
  vector<int> ans = solution.findRedundantDirectedConnection(
      vector<vector<int>>{{1, 2}, {1, 3}, {2, 3}});

  return 0;
}
