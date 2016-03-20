#pragma once

#include "mat.h"
#include "keypoint.h"
#include <vector>
#include <array>

class FastFeatureDetector {
 public:
  FastFeatureDetector(int threshold_, bool non_max_suppression_)
      : threshold(threshold_), non_max_suppression(non_max_suppression_) {}
  void detect(const Mat& img, std::vector<KeyPoint>& keypoints,
              const Mat& mask);
  int threshold;
  bool non_max_suppression;
};

void FAST(const Mat& _img, std::vector<KeyPoint>& keypoints, int threshold,
          bool non_max_suppression);
static void get_circle(int img_cols, std::array<int, 25>& circle);
static void get_thres_tab(int threshold,
                          std::array<unsigned char, 511>& thres_tab);
static int get_score_buf(const unsigned char* v,
                         const std::array<int, 25>& circle);