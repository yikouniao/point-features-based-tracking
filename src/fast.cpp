#include "fast.h"

using namespace cv;

void FastFeatureDetectorT::detect(cv::Mat img, std::vector<cv::KeyPoint>& keypoints, cv::Mat mask) {
  //FAST(img, keypoints, threshold, nonmaxSuppression, type);
  KeyPointsMask(keypoints, mask);
}

bool MaskPredicateT::operator() (const KeyPoint& key_pt) const {
  return mask.at<uchar>((int)(key_pt.pt.y + 0.5f), (int)(key_pt.pt.x + 0.5f)) == 0;
}

void KeyPointsMask(std::vector<cv::KeyPoint>& keypoints, const Mat& mask) {
  if (mask.empty())
    return;

  keypoints.erase(std::remove_if(keypoints.begin(), keypoints.end(), MaskPredicateT(mask)), keypoints.end());
}