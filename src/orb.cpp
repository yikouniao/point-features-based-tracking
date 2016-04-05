#include "orb.h"
#include "fast.h"
#include "file.h"
#include <iostream>
#include <cmath>
#include <algorithm>
#include <iterator>

using namespace std;

OrbMethod::OrbMethod(
    int nfeatures_, size_t nlevels_, float scale_factor_, int fast_threshold_,
    int border_width_, float harris_k_, int patch_size_)
    : nfeatures(nfeatures_), nlevels(nlevels_), scale_factor(scale_factor_),
      fast_threshold(fast_threshold_), border_width(border_width_),
      harris_k(harris_k_), patch_size(patch_size_) {}

void OrbMethod::OrbImpl(
    Mat& img, std::vector<KeyPoint>& keypoints) const {
  vector<Mat> pyramid;

  GetPyramid(img, pyramid);
  GetKeyPoints(pyramid, keypoints);
}

void OrbMethod::GetPyramid(
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

void OrbMethod::GetKeyPoints(const std::vector<Mat>& pyramid,
                                 std::vector<KeyPoint>& keypoints) const {
  keypoints.clear();

  vector<size_t> npts_per_level;
  PtsPerLevel(npts_per_level);

  for (size_t i = 0; i < nlevels; i++) {
    vector<KeyPoint> curr_kpts;

    // FAST detectors
    FastDetector* fd = new FastDetector(fast_threshold, true);
    fd->Detect(pyramid[i], curr_kpts);
    delete fd;

    // Remove keypoints very close to the border
    KeyPointsFilterByImgBorder(curr_kpts, pyramid[i], border_width);

    // Keep more points than necessary
    KeyPointsRetainBest(curr_kpts, npts_per_level[i] * 2);

    // Remove points with lower response by Harris method
    HarrisResponses(curr_kpts, pyramid[i]);
    KeyPointsRetainBest(curr_kpts, npts_per_level[i]);

    // Calculate octave and size of keypoints
    float scale = (float)pow(double(scale_factor), double(i));
    for (auto& e : curr_kpts) {
      e.octave = i;
      e.size = patch_size * scale;
    }

    // Calculate angle of keypoints
    ICAngle(pyramid[i], curr_kpts);

    // Calculate coordinates of keypoints
    for (auto& e : curr_kpts) {
      e.x *= scale;
      e.y *= scale;
    }
    
    keypoints.insert(keypoints.end(), curr_kpts.begin(), curr_kpts.end());
  }
}

void OrbMethod::PtsPerLevel(std::vector<size_t>& npts_per_level) const {
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

void OrbMethod::HarrisResponses(
    std::vector<KeyPoint>& keypoints, const Mat& img, size_t block_size) const {
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

void OrbMethod::ICAngle(
    const Mat& img, std::vector<KeyPoint>& keypoints) const {
  // Compute the end of a row in a circular patch
  int radius = patch_size / 2;
  vector<int> jmax(radius + 1);

  int i_thres_g = (int)floor(radius / sqrt(2.f) + 1);
  int i_thres_l = (int)ceil(radius / sqrt(2.f));

  // row i, column j
  for (int i = 0; i <= i_thres_g; ++i)
    jmax[i] = lround(sqrt((double)radius * radius - i * i));

  // Compute the end of top and bottom rows according to edge columns in a patch
  // to make sure the patch is symmetric
  for (int i = radius, i0 = 0; i >= i_thres_l; --i) {
    while (jmax[i0] == jmax[i0 + 1])
      ++i0;
    jmax[i] = i0++;
  }
  
  // angle = arctan(m_01 / m_10)
  for (auto& e : keypoints) {
    int m_01 = 0, m_10 = 0;

    // the center line, i = 0, m_01 = 0
    for (int j = -radius; j <= radius; ++j)
      m_10 += j * img(lround(e.y), lround(e.x) + j);

    for (int i = 1, i_sum = 0; i <= radius; ++i) {
      // Proceed over the two lines symmetrically
      for (int j = -jmax[i]; j <= jmax[i]; ++j) {
        int i_plus = img(lround(e.y) + i, lround(e.x) + j);
        int i_minus = img(lround(e.y) - i, lround(e.x) + j);
        m_10 += j * (i_plus + i_minus);
        i_sum += (i_plus - i_minus);
      }
      // multiply sum by i here to reduce the number of multiplications
      m_01 += i * i_sum;
    }

    e.angle = OfastAtan((float)m_01, (float)m_10);
  }
}

#define PI 3.14159265359

static float OfastAtan(float y, float x) {
  float theta = float(atan2(y, x) * 180 / PI); // -180 ~ 180
  theta += theta < 0 ? 360 : 0; // 0 ~ 360
  return theta;
}