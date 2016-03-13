#include "fast.h"
#include <vector>

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

void FAST(Mat img, std::vector<KeyPoint>& keypoints, int threshold, bool non_max_suppression) {
  threshold = std::min(std::max(threshold, 0), 255);
  keypoints.clear();
  int circle[25];
  get_circle(img.cols, circle);
  unsigned char threshold_tab[511];
  get_threshold_tab(threshold, threshold_tab);
  std::vector<std::vector<unsigned char>> score_buf(3, std::vector<unsigned char>(img.cols, 0));
  std::vector<std::vector<int>> position_buf(3, std::vector<int>(img.cols, 0));
  std::vector<int> ncorners(3, 0);
}

void get_circle(int img_cols, int* circle) {
  // The first point is three lines before the central point.
  // Other points are arranged clockwise.
  // Continue the circle in order not to miss continuous arcs
  // which include circle[0].
  circle[0] = circle[16] = -img_cols * 3;
  circle[1] = circle[17] = -img_cols * 3 + 1;
  circle[2] = circle[18] = -img_cols * 2 + 2;
  circle[3] = circle[19] = -img_cols + 3;
  circle[4] = circle[20] = 3;
  circle[5] = circle[21] = img_cols + 3;
  circle[6] = circle[22] = img_cols * 2 + 2;
  circle[7] = circle[23] = img_cols * 3 + 1;
  circle[8] = circle[24] = img_cols * 3;
  circle[9] = img_cols * 3 - 1;
  circle[10] = img_cols * 2 - 2;
  circle[11] = img_cols - 3;
  circle[12] = -3;
  circle[13] = -img_cols - 3;
  circle[14] = -img_cols * 2 - 2;
  circle[15] = -img_cols * 3 - 1;
}

void get_threshold_tab(int threshold, unsigned char* threshold_tab) {
  for (size_t i = 0; i < 511; ++i) {
    int vd = i - 255;
    threshold_tab[i] = vd < -threshold ? 1 : vd > threshold ? 2 : 0;
  }
}