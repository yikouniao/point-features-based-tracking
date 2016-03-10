#include "orb.h"
#include "fast.h"

void ORB() {
  cv::Mat img = cv::imread("Witch_Yoshino.png");
  cv::cvtColor(img, img, CV_RGB2GRAY);
  std::vector<cv::KeyPoint> keypoints;
  cv::Mat mask;

  int fastThreshold(20);
  FastFeatureDetectorT* fd = &FastFeatureDetectorT(fastThreshold, true);
  fd->detect(img, keypoints, mask);
}