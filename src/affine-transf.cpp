#include "affine-transf.h"

AffineTransf::AffineTransf() : s(0.f), theta(0.f), tx(0.f), ty(0.f) {}

AffineTransf::AffineTransf(float s_, float theta_, float tx_, float ty_)
    : s(s_), theta(theta_), tx(tx_), ty(ty_) {}

AffineTransf::~AffineTransf() {}

AffineTransf::AffineTransf(const KeyPoint& ref1, const KeyPoint& ref2,
                           const KeyPoint& dst1, const KeyPoint& dst2) {
  float dx_ref = ref1.x - ref2.x, dx_dst = dst1.x - dst2.x;
  float dy_ref = ref1.y - ref2.y, dy_dst = dst1.y - dst2.y;
  s = (dx_dst * dx_dst + dy_dst * dy_dst) /
      (dx_ref * dx_ref + dy_ref * dy_ref);
  s = sqrt(s);
  theta = atan2(dy_dst, dx_dst) - atan2(dy_ref, dx_ref);
  tx = dst2.x - s * (ref2.x * cos(theta) - ref2.y * sin(theta));
  ty = dst2.y - s * (ref2.x * sin(theta) + ref2.y * cos(theta));
}