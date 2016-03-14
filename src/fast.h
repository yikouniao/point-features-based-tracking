#pragma once
#include "opencv2/opencv.hpp"

class FastFeatureDetectorT {
 public:
  FastFeatureDetectorT(int threshold_, bool non_max_suppression_)
      : threshold(threshold_), non_max_suppression(non_max_suppression_) {}
  void detect(const cv::Mat& img, std::vector<cv::KeyPoint>& keypoints, const cv::Mat& mask);
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

static void KeyPointsMask(std::vector<cv::KeyPoint>& keypoints, const cv::Mat& mask);
void FAST(const cv::Mat& _img, std::vector<cv::KeyPoint>& keypoints, int threshold, bool non_max_suppression);
static void get_circle(int img_cols, int* circle);
static void get_threshold_tab(int threshold, unsigned char* threshold_tab);
static int get_score_buf(const unsigned char* v, const int* circle);