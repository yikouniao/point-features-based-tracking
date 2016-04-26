#include "affine-transf.h"
#include <forward_list>

using namespace std;

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

float AffineTransf::get_s() { return s; }
float AffineTransf::get_theta() { return theta; }
float AffineTransf::get_tx() { return tx; }
float AffineTransf::get_ty() { return ty; }

void AffineTransf::set_s(float s_) { s = s_; }
void AffineTransf::set_theta(float theta_) { theta = theta_; }
void AffineTransf::set_tx(float tx_) { tx = tx_; }
void AffineTransf::set_ty(float ty_) { ty = ty_; }

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

void GetAffineTransfImpl(
    const std::vector<AffineTransf>& transf_train, AffineTransf& transf_dst,
    double thresh, int max_weight, int max_pattern_num) {
  vector<forward_list<int>> pattern_idx(1);
  pattern_idx[0].insert_after(pattern_idx[0].before_begin(), 0);
  for (const auto& train : transf_train) {
    // distance
  }
}