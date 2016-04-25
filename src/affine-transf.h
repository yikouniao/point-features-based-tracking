#pragma once

#include "types.h"

struct AffineTransf {
 public:
  // constructors
  AffineTransf();
  AffineTransf(float s_, float theta_, float tx_, float ty_);
  AffineTransf(const KeyPoint& ref1, const KeyPoint& ref2,
               const KeyPoint& dst1, const KeyPoint& dst2);

  // destructor
  ~AffineTransf();

  float s; // scale factor
  float theta; // rotation angle in radians
  float tx; // translation in X-axis
  float ty; // translation in Y-axis
};

// groups matches to calculate affine transformation by sequential clustering
void GetAffineTransf(const std::vector<DescMatch>& matches,
                     AffineTransf& transf);