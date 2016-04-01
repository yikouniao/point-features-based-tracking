#pragma once

#include "types.h"
#include <vector>

class FastFeatureDetector {
 public:
  // constructor
  FastFeatureDetector(int threshold_, bool non_max_suppression_)
      : threshold(threshold_), non_max_suppression(non_max_suppression_) {}

  void Detect(const Mat& img, std::vector<KeyPoint>& keypoints,
              const Mat& mask);

  int threshold;
  bool non_max_suppression;
};

void FAST(const Mat& img, std::vector<KeyPoint>& keypoints, int threshold,
          bool non_max_suppression);
static void get_circle(int img_step, std::vector<int>& circle);
static void get_thres_tab(int threshold,
                          std::vector<unsigned char>& thres_tab);
static int get_score_buf(const unsigned char* v,
                         const std::vector<int>& circle);