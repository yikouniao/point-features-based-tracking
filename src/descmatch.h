#pragma once

// descriptors match info
struct DescMatch {
 public:
  // constructors
  DescMatch();
  DescMatch(int query_idx_, int train_idx_, float distance_);

  int query_idx; // query descriptor index
  int train_idx; // train descriptor index
  float distance;
};

// operator < overload
bool operator <(const DescMatch& m1, const DescMatch& m2);