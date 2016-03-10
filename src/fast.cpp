#include "fast.h"

using namespace cv;

void FastFeatureDetectorT::detect(cv::Mat img, std::vector<cv::KeyPoint>& keypoints, cv::Mat mask) {
  if (img.depth() != CV_8U) {
    std::cerr << "Convert the image into 'unsigned char' first!\n";
    return;
  }

  FAST(img, keypoints, threshold, non_max_suppression);
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

void FAST(Mat _img, std::vector<KeyPoint>& keypoints, int threshold, bool non_max_suppression) {
  int i, j, k, pixel[25];
  threshold = std::min(std::max(threshold, 0), 255);
}