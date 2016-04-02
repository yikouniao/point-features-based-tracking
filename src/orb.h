#pragma once

#include "types.h"
#include <vector>

class ORBDescriptor {
public:
  // constructor
  ORBDescriptor(
    int nfeatures_ = 500, size_t nlevels_ = 8, float scale_factor_ = 1.2f,
    int fast_threshold_ = 20, int border_width_ = 31, float harris_k_ = .04f);

  // ORB
  void Detect(Mat& img, std::vector<KeyPoint>& keypoints) const;

  // calculates pyramid
  void GetPyramid(const Mat& img, std::vector<Mat>& pyramid) const;

  // calculates oFAST keypoints
  void GetKeyPoints(const std::vector<Mat>& pyramid,
                    std::vector<KeyPoint>& keypoints) const;

  // calculates how many keypoints each level should have
  void PtsPerLevel(std::vector<size_t>& npts_per_level) const;

  // calculates Harris responses for keypoints
  void HarrisResponses(
      std::vector<KeyPoint>& keypoints, const Mat& img,
      size_t block_size = 7) const;

  int nfeatures;
  size_t nlevels;
  float scale_factor;
  int fast_threshold;
  int border_width;
  float harris_k;
};