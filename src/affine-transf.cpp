#include "affine-transf.h"
#include <forward_list>
#include <algorithm>
#include <cfloat>

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

AffineTransf::AffineTransf(const Pointf& ref1, const Pointf& ref2,
                           const Pointf& dst1, const Pointf& dst2) {
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

void AffineTransf::GetDstPoint(const Pointf& ref, Pointf& dst) {
  dst.x = tx + s * (ref.x * cos(theta) - ref.y * sin(theta));
  dst.y = ty + s * (ref.x * sin(theta) + ref.y * cos(theta));
}

AffineTransf& AffineTransf::operator +=(const AffineTransf& transf) {
  s += transf.s;
  theta += transf.theta;
  tx += transf.tx;
  ty += transf.ty;
  return *this;
}

AffineTransf& AffineTransf::operator *=(int n) {
  s *= n;
  theta *= n;
  tx *= n;
  ty *= n;
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

AffineTransf operator +(const AffineTransf& t1, const AffineTransf& t2) {
  return AffineTransf{t1.s + t2.s, t1.theta + t2.theta,
                      t1.tx + t2.tx, t1.ty + t2.ty };
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

AffineTransf GetAffineTransf(
    const std::vector<KeyPoint>& kps_ref, const std::vector<KeyPoint>& kps_dst,
    const std::vector<DescMatch>& matches) {
  vector<AffineTransf> transf_train;

// judges whether 2 points in reference or dstination image are the same
#define NOT_SAME_POINTS(i, j) \
    (EuclidDistSquare(kps_ref[matches[i].idx_query].point, \
                      kps_ref[matches[j].idx_query].point) > 1e-4 && \
     EuclidDistSquare(kps_dst[matches[i].idx_train].point, \
                      kps_dst[matches[j].idx_train].point) > 1e-4)

// initialization list for class AffineTransf
#define GET_POINTS(i, j) \
    {kps_ref[matches[i].idx_query].point, kps_ref[matches[j].idx_query].point,\
     kps_dst[matches[i].idx_train].point, kps_dst[matches[j].idx_train].point}

  // If there're few matchse, select matches by C(n, 2);
  size_t matches_size = matches.size();
  if (matches_size < 30) {
    for (size_t i = 0; i < matches_size - 1; ++i) {
      for (size_t j = i + 1; j < matches_size; ++j) {
        if (NOT_SAME_POINTS(i, j))
          transf_train.push_back(GET_POINTS(i, j));
      }
    }
  } else { // else go sequentially, i.e. (point1, point2), (point2, point3)
    for (size_t i = 0; i < matches_size - 1; ++i) {
      if (NOT_SAME_POINTS(i, i + 1))
        transf_train.push_back(GET_POINTS(i, i + 1));
    }
  }

  return GetAffineTransfImpl(transf_train);
}

static AffineTransf GetAffineTransfImpl(
    const std::vector<AffineTransf>& transf_train, float thresh,
    float max_weight_factor, size_t max_pattern_num) {
  AffineTransf var{GetVar(transf_train)}; // Calculate variance
  vector<AffineTransf> patterns(1, transf_train[0]);
  vector<int> weights(1, 1);
  size_t transf_train_size = transf_train.size();
  int max_weight = max(1, int(max_weight_factor * transf_train_size));

  for (size_t i = 1; i < transf_train_size; ++i) {
    float dist_min = FLT_MAX;
    size_t dist_min_idx = -1;
    // Compute Mahalanobis distance between current point and processed points
    for (size_t j = 0; j < patterns.size(); ++j) {
      float dist = MahDistance(transf_train[i], patterns[j], var);
      if (dist < dist_min) {
        dist_min = dist;
        dist_min_idx = j;
      }
    }

#define GET_CENTROID(idx) \
    {((patterns[idx] *= (weights[idx])++) += transf_train[i]) /= weights[idx];}
    
    if (dist_min < thresh || weights.size() >= max_pattern_num) {
      // Get new centroid and add 1 to weight correspondingly
      GET_CENTROID(dist_min_idx);
      // If the weight of a pattern is large enough, take it as the result.
      if (weights[dist_min_idx] > max_weight)
        return patterns[dist_min_idx];
    } else {
      // add a new pattern
      patterns.push_back(transf_train[i]);
      weights.push_back(1);
    }
  }

  return *(max_element(weights.cbegin(), weights.cend()) - weights.cbegin() +
           patterns.cbegin());
}