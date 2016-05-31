#pragma once

#include "types.h"
#include <vector>
#include <array>

using OrbDescriptors = std::vector<std::array<uchar, 32>>;

class OrbMethod {
public:
  // constructor
  OrbMethod(
      int nfeatures_ = 500, size_t nlevels_ = 8, float scale_factor_ = 1.2f,
      int fast_threshold_ = 20, int border_width_ = 31, float harris_k_ = .04f,
      int patch_size_ = 31);

  // calculates ORB detectors and descriptors
  void OrbImpl(Mat& img, std::vector<KeyPoint>& keypoints,
               OrbDescriptors& descriptors) const;

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

  // calculates oFAST feature orientation (in degree) by intensity centroid
  void ICAngle(const Mat& img, std::vector<KeyPoint>& keypoints) const;

  // calculates rBRIEF descriptors
  void GetDescriptors(const std::vector<Mat>& pyramid,
                      const std::vector<KeyPoint>& keypoints,
                      OrbDescriptors& descriptors) const;

  int nfeatures; // quantity of features
  size_t nlevels; // amount of levels
  float scale_factor; // factor for generating image pyramid
  int fast_threshold; // threshold for FAST keypoints detection
  // the width of the border where the features are not detected
  // should roughly match patch_size
  int border_width;
  float harris_k; // k for Harris response
  int patch_size; // patch size for rBRIEF, larger in higher levels
};

// return in degree, range in 0 ~ 360
static double OfastAtan(double y, double x);

// matches orb descriptors
void OrbMatch(
    const OrbDescriptors& desc_query, const OrbDescriptors& desc_train,
    std::vector<DescMatch>& matches);