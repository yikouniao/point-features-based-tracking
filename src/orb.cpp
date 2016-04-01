#include "orb.h"
#include "fast.h"
#include "file.h"
#include <iostream>
#include <cmath>
#include <algorithm>

using namespace std;

ORBDescriptor::ORBDescriptor(
    int nfeatures_, size_t nlevels_, float scale_factor_,
    int fast_threshold_, int border_width_, float harris_k_)
    : nfeatures(nfeatures_), nlevels(nlevels_), scale_factor(scale_factor_),
      fast_threshold(fast_threshold_), border_width(border_width_),
      harris_k(harris_k_) {}

void ORBDescriptor::Detect(const Mat& img) const {
  vector<Mat> pyramid;
  vector<KeyPoint> keypoints;

  GetPyramid(img, pyramid);
  GetKeyPoints(pyramid, keypoints);
}

void ORBDescriptor::GetPyramid(
    const Mat& img, std::vector<Mat>& pyramid) const {
  // Clear the pyramid
  for (size_t i = 0; i < pyramid.size(); ++i) {
    pyramid[i].Release();
  }
  pyramid.clear();

  // Construct the pyramid
  pyramid.push_back(img);
  for (size_t i = 1; i < nlevels; ++i) {
    pyramid.push_back(Resize(pyramid[i - 1], scale_factor, scale_factor));
  }
}

void ORBDescriptor::GetKeyPoints(const std::vector<Mat>& pyramid,
                                 std::vector<KeyPoint>& keypoints) const {
  Mat mask;

  vector<size_t> npts_per_level;
  PtsPerLevel(npts_per_level);

  for (size_t i = 0; i < nlevels; i++) {
    // FAST detectors
    FastFeatureDetector* fd = new FastFeatureDetector(fast_threshold, true);
    fd->Detect(pyramid[i], keypoints, mask);
    delete fd;

    // Remove keypoints very close to the border
    KeyPointsFilterByImgBorder(keypoints, pyramid[i], border_width);

    // Keep more points than necessary
    KeyPointsRetainBest(keypoints, npts_per_level[i] * 2);
    
    /// octave in FAST keypoints, pending
  }

  mask.Release();
}

void ORBDescriptor::PtsPerLevel(std::vector<size_t>& npts_per_level) const {
  npts_per_level.clear();
  npts_per_level.resize(nlevels);

  // pts_in_1st_level*(1 + scale_factor +...+ scale_factor^(n-1)) = nfeatures
  float factor = 1 / scale_factor;
  float ndesired_pts_per_level = nfeatures * (1 - factor) /
      (1 - (float)pow(double(factor), double(nlevels)));

  int sum_features = 0;
  for (size_t i = 0; i < nlevels - 1; i++) {
    npts_per_level[i] = lround(ndesired_pts_per_level);
    sum_features += npts_per_level[i];
    ndesired_pts_per_level *= factor;
  }
  npts_per_level[nlevels - 1] = max(nfeatures - sum_features, 0);
}

void ORBDescriptor::HarrisResponses(std::vector<KeyPoint>& keypoints,
                                    const Mat& img, size_t block_size) const {
  if (img.rows < block_size || img.cols < block_size)
    return;

  for (auto& e : keypoints) {
    int Ix_2 = 0 /* Ix^2 */, Iy_2 = 0 /* Iy^2 */, IxIy = 0 /* Ix*Iy */;
    int y0 = (int)e.y - block_size / 2;
    int x0 = (int)e.x - block_size / 2;
    for (size_t i = 0; i < block_size; ++i) {
      for (size_t j = 0; j < block_size; ++j) {
        int y = y0 + i;
        int x = x0 + j;
        // x gradient, Sobel
        int Ix = img(y - 1, x + 1) - img(y - 1, x - 1) +
                 (img(y, x + 1) - img(y0, x - 1)) * 2 +
                 img(y + 1, x + 1) - img(y + 1, x - 1);
        // y gradient, Sobel
        int Iy = img(y + 1, x - 1) - img(y - 1, x - 1) +
                 (img(y + 1, x) - img(y - 1, x)) * 2 +
                 img(y + 1, x + 1) - img(y - 1, x + 1);
        Ix_2 += Ix * Ix;
        Iy_2 += Iy * Iy;
        IxIy += Ix * Iy;
      }
    }
    // M = sum([Ix^2, IxIy; IxIy, Iy^2])
    // response = det(M) - k(trace(M))^2
    e.response = Ix_2 * Iy_2 - IxIy * IxIy -
                 harris_k * (Ix_2 + Iy_2) * (Ix_2 + Iy_2);
  }
}