#pragma once

#include "types.h"
#include <vector>

class AffineTransf {
 public:
  // constructors
  AffineTransf();
  AffineTransf(float s_, float theta_, float tx_, float ty_);
  AffineTransf(const AffineTransf& transf_);
  AffineTransf(const Pointf& ref1, const Pointf& ref2,
               const Pointf& dst1, const Pointf& dst2);

  // destructor
  ~AffineTransf();

  // gets destination point by affine transformation
  void GetDstPoint(const Pointf& ref, Pointf& dst);

  // operators overloading
  AffineTransf& operator +=(const AffineTransf& transf);
  AffineTransf& operator *=(int n);
  AffineTransf& operator /=(int n);

  // pow(*this, 2)
  AffineTransf& Square();

  float s; // scale factor
  float theta; // rotation angle in radians
  float tx; // translation in X-axis
  float ty; // translation in Y-axis
};

// operators overloading
AffineTransf operator +(const AffineTransf& t1, const AffineTransf& t2);
AffineTransf operator -(const AffineTransf& t1, const AffineTransf& t2);

// calculates variance
AffineTransf GetVar(const std::vector<AffineTransf>& transfs);

// returns Mahalanobis distance
float MahDistance(const AffineTransf& r1, const AffineTransf& r2,
                  const AffineTransf& var);

// groups matches to calculate affine transformation by sequential clustering
AffineTransf GetAffineTransf(
    const std::vector<KeyPoint>& kps_ref, const std::vector<KeyPoint>& kps_dst,
    const std::vector<DescMatch>& matches);

// gets affine transformation by sequential clustering
static AffineTransf GetAffineTransfImpl(
    const std::vector<AffineTransf>& transf_train, float thresh = .015f,
    float max_weight_factor = .1f, size_t max_pattern_num = 100);