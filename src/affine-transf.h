#pragma once

#include "types.h"
#include <vector>

class AffineTransf {
 public:
  // constructors
  AffineTransf();
  AffineTransf(float s_, float theta_, float tx_, float ty_);
  AffineTransf(const KeyPoint& ref1, const KeyPoint& ref2,
               const KeyPoint& dst1, const KeyPoint& dst2);

  // destructor
  ~AffineTransf();

  // accessors and mutators
  float get_s();
  float get_theta();
  float get_tx();
  float get_ty();

  void set_s(float s_);
  void set_theta(float theta_);
  void set_tx(float tx_);
  void set_ty(float ty_);

  // returns Mahalanobis distance
  friend double MahDistance(const AffineTransf& r1, const AffineTransf& r2,
                            const AffineTransf& var);

 private:
  float s; // scale factor
  float theta; // rotation angle in radians
  float tx; // translation in X-axis
  float ty; // translation in Y-axis
};

// returns Mahalanobis distance
double MahDistance(const AffineTransf& r1, const AffineTransf& r2,
                   const AffineTransf& var) {
  return (r1.s - r2.s) * (r1.s - r2.s) / var.s +
         (r1.tx - r2.tx) * (r1.tx - r2.tx) / var.tx +
         (r1.ty - r2.ty) * (r1.ty - r2.ty) / var.ty +
         (r1.theta - r2.theta) * (r1.theta - r2.theta) / var.theta;
}

// groups matches to calculate affine transformation by sequential clustering
void GetAffineTransf(const std::vector<DescMatch>& matches,
                     AffineTransf& transf);

// gets affine transformation by sequential clustering
void GetAffineTransfImpl(
    const std::vector<AffineTransf>& transf_train, AffineTransf& transf_dst,
    double thresh = 2.0e-13, int max_weight = 50, int max_pattern_num = 10);