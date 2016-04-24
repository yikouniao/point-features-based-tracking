#include "descmatch.h"
#include <cfloat>

DescMatch::DescMatch() {
  idx_query = idx_train = -1;
  distance = FLT_MAX;
}

DescMatch::DescMatch(int idx_query_, int idx_train_, float distance_)
    : idx_query(idx_query_), idx_train(idx_train_), distance(distance_) {}

DescMatch::~DescMatch() {}

bool operator <(const DescMatch& m1, const DescMatch& m2) {
  return m1.distance < m2.distance;
}

int NormHamming(const unsigned char* a, const unsigned char* b, int n) {
  int result = 0;
  for (int i = 0; i < n; ++i)
    result += pop_count[a[i] ^ b[i]];
  return result;
}