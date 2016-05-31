#pragma once

// descriptors match info
struct DescMatch {
 public:
  // constructors
  DescMatch();
  DescMatch(int idx_query_, int idx_train_, float distance_);

  // destructor
  ~DescMatch();

  int idx_query; // query descriptor index
  int idx_train; // train descriptor index
  float distance;
};

// operator < overload
bool operator <(const DescMatch& m1, const DescMatch& m2);

// returns Hamming distance
int NormHamming(const unsigned char* a, const unsigned char* b, int n);