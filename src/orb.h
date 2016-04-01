#pragma once

#include "types.h"
#include <vector>

class ORBDescriptor {
public:
  // constructor
  ORBDescriptor(
    int nfeatures_ = 500, size_t nlevels_ = 8, float scale_factor_ = 1.2f,
    int fast_threshold_ = 20, int border_width_ = 31, float harris_k_ = .04f);

  void Detect(const Mat& img) const;
  void GetPyramid(const Mat& img, std::vector<Mat>& pyramid) const;
  void GetKeyPoints(const std::vector<Mat>& pyramid,
                    std::vector<KeyPoint>& keypoints) const;
  void PtsPerLevel(std::vector<size_t>& npts_per_level) const;
  void HarrisResponses(std::vector<KeyPoint>& keypoints, const Mat& img,
                       size_t block_size = 7) const;

  int nfeatures;
  size_t nlevels;
  float scale_factor;
  int fast_threshold;
  int border_width;
  float harris_k;
};