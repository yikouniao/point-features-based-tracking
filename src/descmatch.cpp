#include "descmatch.h"

DescMatch::DescMatch() {
  query_idx = train_idx = -1;
  distance = -1.f;
}

DescMatch::DescMatch(int query_idx_, int train_idx_, float distance_)
    : query_idx(query_idx_), train_idx(train_idx_), distance(distance_) {}

bool operator <(const DescMatch& m1, const DescMatch& m2) {
  return m1.distance < m2.distance;
}