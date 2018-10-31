// Copyright 2018 Chen Zou. All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#ifndef ALGORITHM_BINARY_INDEXED_TREE_H_
#define ALGORITHM_BINARY_INDEXED_TREE_H_

#include <vector>

namespace algorithm {
template<class T> class BinarayIndexedTree {
 private:
  ::std::vector<T> s_;

 public:
  explicit BinarayIndexedTree(::std::vector<T> a) {
    int n = a.size() + 1;
    s_.resize(n);
    for (int i = 0; i < n; i++) {
      s_[i] = 0;
    }
    for (int i = 0; i < n; i++) {
      add(i, a[i]);
    }
  }
  T sum(int i) {
    T ans(0);
    while (i > 0) {
      ans += s_[i];
      i -= lowbit(i);
    }
    return ans;
  }
  void add(int i, T v) {
    i++;
    while (i < s_.size()) {
      s_[i] += v;
      i += lowbit(i);
    }
  }

 private:
  inline static int lowbit(int x) {
    return x & -x;
  }
};

};  // namespace algorithm

#endif  // ALGORITHM_BINARY_INDEXED_TREE_H_
