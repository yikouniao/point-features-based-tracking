#pragma once
#include "opencv2/opencv.hpp"

class FastFeatureDetectorT {
 public:
  FastFeatureDetectorT(int threshold_, bool non_max_suppression_)
      : threshold(threshold_), non_max_suppression(non_max_suppression_) {}
  void detect(cv::Mat img, std::vector<cv::KeyPoint>& keypoints, cv::Mat mask);
  int threshold;
  bool non_max_suppression;
};

class MaskPredicateT {
public:
  MaskPredicateT(const cv::Mat& mask_) : mask(mask_) {}
  bool operator() (const cv::KeyPoint& key_pt) const;

private:
  const cv::Mat mask;
};

void KeyPointsMask(std::vector<cv::KeyPoint>& keypoints, const cv::Mat& mask);