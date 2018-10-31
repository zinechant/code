// Copyright_ 2018 Chen Zou. All right_s reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#ifndef ALGORITHM_SPLAY_TREE_H_
#define ALGORITHM_SPLAY_TREE_H_

#include <functional>

namespace algorithm {

template <class T, class Node, class Compare>
class SplayTree;

template <class T, class Compare = std::less<T> >
class SplayTreeNode {
 public:
  explicit SplayTreeNode(T key)
      : key_(key), size_(1), parent_(NULL), left_(NULL), right_(NULL) {}
  virtual ~SplayTreeNode() {}
  const T &key() { return key_; }

 private:
  T key_;
  size_t size_;

  SplayTreeNode *parent_;
  SplayTreeNode *left_;
  SplayTreeNode *right_;

  virtual void ApplyTags() {}
  virtual void Update() {
    size_ = 1;
    if (left_) size_ += left_->size_;
    if (right_) size_ += right_->size_;
  }
  friend class SplayTree<T, SplayTreeNode<T, Compare>, Compare>;
};

template <class T, class Node = SplayTreeNode<T>, class Compare = std::less<T> >
class SplayTree {
 public:
  SplayTree() : root_(NULL) {}
  Node *root() { return root_; }

  Node *subtree_minimum(Node *x = NULL) {
    if (x == NULL) x = root_;
    while (x->left_) x = x->left_;
    return x;
  }

  Node *subtree_maximum(Node *x = NULL) {
    if (x == NULL) x = root_;
    while (x->right_) x = x->right_;
    return x;
  }

  Node *find(const T &key) {
    Node *x = root_;
    while (x) {
      x->ApplyTags();
      if (compare(x->key_, key)) {
        x = x->right_;
      } else if (compare(key, x->key_)) {
        x = x->left_;
      } else {
        splay(x);
        return x;
      }
    }
    return NULL;
  }

  // kth of the subtree in the tree order. !!! Starts from 0 !!!
  Node *find_subtree_kth(size_t kth, Node *x = NULL) {
    if (x == NULL) x = root_;

    kth++;

    if (!x) {
      return NULL;
    }

    if (kth > x->size_) {
      return NULL;
    }

    while (x) {
      x->ApplyTags();

      size_t smaller_size = 1;
      if (x->left_) {
        smaller_size += x->left_->size_;
      }

      if (kth == smaller_size) {
        return x;
      } else if (kth > smaller_size) {
        kth -= smaller_size;
        x = x->right_;
      } else {
        x = x->left_;
      }
    }
    return NULL;
  }

  void insert(const T &key) {
    Node *x = root_;
    Node *p = NULL;

    while (x) {
      x->ApplyTags();

      p = x;
      if (compare(x->key_, key))
        x = x->right_;
      else
        x = x->left_;
    }

    x = new Node(key);
    x->parent_ = p;

    if (!p) {
      root_ = x;
    } else if (compare(p->key_, x->key_)) {
      p->right_ = x;
    } else {
      p->left_ = x;
    }

    x->Update();
    if (p) p->Update();

    splay(x);
  }

  // kth of the subtree in the tree order. !!! Starts from 0 !!!
  Node *insert_after_kth(T key, size_t kth, Node *r = NULL) {
    if (r == NULL) {
      r = root_;
    }
    Node *x = find_subtree_kth(kth, r);
    if (!x) return NULL;
    splay(x, r->parent_);
    Node *y = new Node(key);
    y->parent_ = x;
    y->left_ = NULL;
    y->right_ = x->right_;

    if (x->right_) {
      x->right_->parent_ = y;
    }

    x->right_ = y;

    y->Update();
    x->Update();
    return y;
  }

  void erase(Node *x) {
    if (!x) return;

    splay(x);

    Node *s = x->left_;
    Node *t = x->right_;

    delete x;

    Node *sMax = NULL;
    if (s) {
      s->parent_ = NULL;
      sMax = subtree_maximum(s);
      splay(sMax);
      sMax->right_ = t;
    }

    if (t) {
      t->parent_ = sMax;
    }

    if (s) {
      root_ = sMax;
    } else {
      root_ = t;
    }

    if (t) t->Update();
    if (sMax) sMax->Update();
  }

  void erase(const T &key) {
    Node *x = find(key);
    erase(x);
  }

  // kth starting at 0 !!!
  size_t kth_smaller(Node *x) {
    splay(x);
    size_t ans = 0;
    if (x->left_) ans = x->left_->size_;
    return ans;
  }

  // kth starting at 0 !!!
  size_t kth_larger(Node *x) {
    splay(x);
    size_t ans = 0;
    if (x->right_) ans = x->right_->size_;
    return ans;
  }

  bool empty() { return root_->size_ == 0; }
  size_t size() { return root_->size_; }

 private:
  Compare compare;
  Node *root_;

  void left_rotate(Node *x) {
    Node *y = x->right_;
    if (y) {
      x->right_ = y->left_;
      if (y->left_) y->left_->parent_ = x;
      y->parent_ = x->parent_;
    }

    if (!x->parent_)
      root_ = y;
    else if (x == x->parent_->left_)
      x->parent_->left_ = y;
    else
      x->parent_->right_ = y;
    if (y) y->left_ = x;
    x->parent_ = y;

    x->Update();
    if (y) y->Update();
  }

  void right_rotate(Node *x) {
    Node *y = x->left_;
    if (y) {
      x->left_ = y->right_;
      if (y->right_) y->right_->parent_ = x;
      y->parent_ = x->parent_;
    }
    if (!x->parent_)
      root_ = y;
    else if (x == x->parent_->left_)
      x->parent_->left_ = y;
    else
      x->parent_->right_ = y;
    if (y) y->right_ = x;
    x->parent_ = y;

    x->Update();
    if (y) y->Update();
  }

  void splay(Node *x, Node *subtree_parent = NULL) {
    while (x->parent_ != subtree_parent) {
      if (x->parent_->parent_ == subtree_parent) {
        if (x->parent_->left_ == x)
          right_rotate(x->parent_);
        else
          left_rotate(x->parent_);
      } else if (x->parent_->left_ == x &&
                 x->parent_->parent_->left_ == x->parent_) {
        right_rotate(x->parent_->parent_);
        right_rotate(x->parent_);
      } else if (x->parent_->right_ == x &&
                 x->parent_->parent_->right_ == x->parent_) {
        left_rotate(x->parent_->parent_);
        left_rotate(x->parent_);
      } else if (x->parent_->left_ == x &&
                 x->parent_->parent_->right_ == x->parent_) {
        right_rotate(x->parent_);
        left_rotate(x->parent_);
      } else {
        left_rotate(x->parent_);
        right_rotate(x->parent_);
      }
    }
  }
};

}  // namespace algorithm

#endif  // ALGORITHM_SPLAY_TREE_H_
