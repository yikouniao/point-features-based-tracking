#pragma once

#include "types.h"
#include <vector>

class AffineTransf {
 public:
  // constructors
  AffineTransf();
  AffineTransf(float s_, float theta_, float tx_, float ty_);
  AffineTransf(const AffineTransf& transf_);
  AffineTransf(const KeyPoint& ref1, const KeyPoint& ref2,
               const KeyPoint& dst1, const KeyPoint& dst2);

  // destructor
  ~AffineTransf();

  // operators overloading
  AffineTransf& operator +=(const AffineTransf& transf);
  AffineTransf& operator /=(int n);

  // pow(*this, 2)
  AffineTransf& Square();

  float s; // scale factor
  float theta; // rotation angle in radians
  float tx; // translation in X-axis
  float ty; // translation in Y-axis
};

// operators overloading
AffineTransf operator -(const AffineTransf& t1, const AffineTransf& t2);

// calculates variance
AffineTransf GetVar(const std::vector<AffineTransf>& transfs);

// returns Mahalanobis distance
float MahDistance(const AffineTransf& r1, const AffineTransf& r2,
                  const AffineTransf& var);

// groups matches to calculate affine transformation by sequential clustering
void GetAffineTransf(const std::vector<DescMatch>& matches,
                     AffineTransf& transf);

// gets affine transformation by sequential clustering
static void GetAffineTransfImpl(
    const std::vector<AffineTransf>& transf_train, AffineTransf& transf_dst,
    float thresh = 2.0e-13f, int max_weight = 50, int max_pattern_num = 10);