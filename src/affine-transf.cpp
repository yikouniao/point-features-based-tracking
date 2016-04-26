#include "affine-transf.h"
#include <forward_list>
#include <algorithm>

using namespace std;

AffineTransf::AffineTransf() : s(0.f), theta(0.f), tx(0.f), ty(0.f) {}

AffineTransf::AffineTransf(float s_, float theta_, float tx_, float ty_)
    : s(s_), theta(theta_), tx(tx_), ty(ty_) {}

AffineTransf::AffineTransf(const AffineTransf& transf) {
  s = transf.s;
  theta = transf.theta;
  tx = transf.tx;
  ty = transf.ty;
}

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

AffineTransf::~AffineTransf() {}

AffineTransf& AffineTransf::operator +=(const AffineTransf& transf) {
  s += transf.s;
  theta += transf.theta;
  tx += transf.tx;
  ty += transf.ty;
  return *this;
}

AffineTransf& AffineTransf::operator /=(int n) {
  s /= n;
  theta /= n;
  tx /= n;
  ty /= n;
  return *this;
}

AffineTransf& AffineTransf::Square() {
  s *= s;
  theta *= theta;
  tx *= tx;
  ty *= ty;
  return *this;
}

AffineTransf operator -(const AffineTransf& t1, const AffineTransf& t2) {
  return AffineTransf{t1.s - t2.s, t1.theta - t2.theta,
                      t1.tx - t2.tx, t1.ty - t2.ty};
}

AffineTransf GetVar(const std::vector<AffineTransf>& transfs) {
  AffineTransf mean;
  for (const auto& transf : transfs)
    mean += transf;
  mean /= int(transfs.size());

  AffineTransf var;
  for (const auto& transf : transfs) {
    AffineTransf temp{transf - mean};
    var += temp.Square();
  }
  var /= int(transfs.size() - 1);

  return var;
}

float MahDistance(const AffineTransf& r1, const AffineTransf& r2,
                  const AffineTransf& var) {
  return (r1.s - r2.s) * (r1.s - r2.s) / var.s +
    (r1.tx - r2.tx) * (r1.tx - r2.tx) / var.tx +
    (r1.ty - r2.ty) * (r1.ty - r2.ty) / var.ty +
    (r1.theta - r2.theta) * (r1.theta - r2.theta) / var.theta;
}

void GetAffineTransf(
    const std::vector<KeyPoint>& kps_ref, const std::vector<KeyPoint>& kps_dst,
    const std::vector<DescMatch>& matches, AffineTransf& transf) {
  vector<AffineTransf> transf_train;

#define GET_POINTS(i, j) \
    {kps_ref[matches[i].idx_query], kps_ref[matches[j].idx_query], \
     kps_dst[matches[i].idx_train], kps_dst[matches[j].idx_train]}

  // If there're few matchse, select matches by C(n, 2);
  if (matches.size() < 30) {
    for (size_t i = 0; i < matches.size() - 1; ++i) {
      for (size_t j = i + 1; j < matches.size(); ++j) {
        transf_train.push_back(GET_POINTS(i, j));
      }
    }
  } else { // else go sequentially, i.e. (point1, point2), (point2, point3)
    for (size_t i = 0; i < matches.size(); ++i) {
      transf_train.push_back(GET_POINTS(i, i + 1));
    }
  }

  GetAffineTransfImpl(transf_train, transf);
}

static void GetAffineTransfImpl(
    const std::vector<AffineTransf>& transf_train, AffineTransf& transf_dst,
    double thresh, int max_weight, int max_pattern_num) {
  vector<forward_list<int>> pattern_idx(1);
  pattern_idx[0].insert_after(pattern_idx[0].before_begin(), 0);
  for (const auto& train : transf_train) {
    // distance
  }
}