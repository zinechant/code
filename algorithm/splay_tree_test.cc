// Copyright 2018 Chen Zou. All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <cstdio>
#include <cstdlib>
#include <unordered_map>
#include <utility>
#include <vector>

#include "algorithm/splay_tree.h"
#include "gtest/gtest.h"

TEST(BinarayIndexedTreeTest, SimpleTest) {
  typedef int my_int;
  const my_int N = 1000000;

  // std::vector<my_int> ref;
  algorithm::SplayTree<my_int> splay_tree;

  for (my_int i = 0; i < N; i += 2) {
    // ref.push_back(i);
    splay_tree.insert(i);

    const auto node = splay_tree.find(i);
    EXPECT_NE(nullptr, node);
    EXPECT_EQ(i, node->key());
    EXPECT_EQ(i >> 1, splay_tree.kth_smaller(node));
  }

  for (my_int i = 0; i < N; i += 2) {
    const auto node = splay_tree.find(i);
    EXPECT_NE(nullptr, node);
    EXPECT_EQ(i, node->key());

    EXPECT_EQ(i >> 1, splay_tree.kth_smaller(node));
    EXPECT_EQ((N - i) >> 1, 1 + splay_tree.kth_larger(node));
  }

  for (my_int i = 1; i < N; i += 4) {
    splay_tree.insert(i);

    const auto node = splay_tree.find(i);
    EXPECT_NE(nullptr, node);
    EXPECT_EQ(i, node->key());
    EXPECT_EQ((i >> 1) + (i >> 2) + 1, splay_tree.kth_smaller(node));
  }

  for (my_int i = 3; i < N; i += 4) {
    const auto ret_node = splay_tree.insert_after_kth(i, i - 1);
    EXPECT_NE(nullptr, ret_node);

    const auto node = splay_tree.find(i);
    EXPECT_EQ(ret_node, node);
    EXPECT_NE(nullptr, node);
    EXPECT_EQ(i, node->key());
    EXPECT_EQ(i, splay_tree.kth_smaller(node));
  }

  for (my_int i = 0; i < N; i++) {
    const auto node = splay_tree.find(i);
    EXPECT_NE(nullptr, node);
    EXPECT_EQ(i, node->key());

    EXPECT_EQ(i, splay_tree.kth_smaller(node));
    EXPECT_EQ(N - i, 1 + splay_tree.kth_larger(node));
  }

  for (my_int i = N - 1; i >= 0; i -= 2) {
    splay_tree.erase(i);
    const auto node = splay_tree.find(i - 1);
    EXPECT_NE(nullptr, node);
    EXPECT_EQ(i - 1, node->key());

    EXPECT_EQ((N - i - 1) >> 1, splay_tree.kth_larger(node));
  }
}

const int KnobMaxDistance = 1000000;
::std::unordered_map<uint64_t, uint64_t> reuse_distance;
::std::unordered_map<uint64_t, uint64_t> last_access_id;
algorithm::SplayTree<std::pair<uint64_t, uint64_t> > last_accesses;

uint64_t access_id = 0;
bool exceeded = false;

void calculate_reuse_analysis(uint64_t addr, uint64_t expect) {
  if (exceeded) return;
  if (access_id + 1 == 0) {
    printf(
        "Exceed (1 << 64) accesses, only first (1 << 64) accesses analyzed.\n");
    exceeded = true;
    return;
  }

  ::std::unordered_map<uint64_t, uint64_t>::iterator iterator =
      last_access_id.find(addr);
  if (iterator == last_access_id.end()) {
    reuse_distance[KnobMaxDistance] += 1;
    EXPECT_EQ(expect, KnobMaxDistance);
    last_access_id[addr] = access_id;
    last_accesses.insert(std::pair<uint64_t, uint64_t>(access_id, addr));
  } else {
    uint64_t old_id = last_access_id[addr];
    last_access_id[addr] = access_id;
    algorithm::SplayTreeNode<std::pair<uint64_t, uint64_t> > *node =
        last_accesses.find(std::pair<uint64_t, uint64_t>(old_id, addr));
    uint64_t distance = last_accesses.kth_larger(node);

    EXPECT_EQ(expect, distance);

    last_accesses.erase(node);
    last_accesses.insert(std::pair<uint64_t, uint64_t>(access_id, addr));
    if (reuse_distance.count(distance) == 0) {
      reuse_distance[distance] = 1;
    } else {
      reuse_distance[distance] += 1;
    }
  }
  while (last_accesses.size() > KnobMaxDistance) {
    const std::pair<uint64_t, uint64_t> &minimum_pair =
        last_accesses.subtree_minimum()->key();
    ::std::unordered_map<uint64_t, uint64_t>::iterator iterator =
        last_access_id.find(minimum_pair.second);
    assert(iterator->second == minimum_pair.first);
    last_access_id.erase(iterator);
    last_accesses.erase(minimum_pair);
  }

  access_id++;
}

TEST(BinarayIndexedTreeTest, ReuseDistanceTest) {
  const int N = 1000;
  for (int i = 0; i < N; i++) {
    calculate_reuse_analysis(i, KnobMaxDistance);
  }
  for (int k = 0; k < N; k++)
    for (int i = 0; i < N; i++) {
      calculate_reuse_analysis(i, N - 1);
    }
}
