// Copyright 2018 Chen Zou. All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <cstdio>
#include <cstdlib>

#include "algorithm/binary_indexed_tree.h"
#include "gtest/gtest.h"

TEST(BinarayIndexedTreeTest, RandomTest) {
  const int N = 1000;
  const int M = 1000;
  int a[N];
  unsigned int seed = 0;
  for (int i = 0; i < N; i++) {
    a[i] = rand_r(&seed) % M;
  }

  ::algorithm::BinarayIndexedTree<int> stree(std::vector<int>(a, a + N));

  for (int i = 0; i < M; i++) {
    int x = rand_r(&seed) % N;
    int sum = 0;
    for (int i = 0; i < x; i++) {
      sum += a[i];
    }

    EXPECT_EQ(sum, stree.sum(x));
  }
}
