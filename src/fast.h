#pragma once

#include "types.h"
#include <vector>

class FastDetector {
 public:
  // constructor
  FastDetector(int threshold_, bool non_max_suppression_)
      : threshold(threshold_), non_max_suppression(non_max_suppression_) {}

  // detector
  void Detect(const Mat& img, std::vector<KeyPoint>& keypoints);

  // threshold for intensity difference of central point and circular points
  int threshold;
  // true if needs non-maximum suppression
  bool non_max_suppression;
};

// computes memory address offsets for circular points to central point
// The first point is three lines right before the central point.
// Other points are arranged clockwise.
// Continue the circle in order not to miss continuous arcs
// which include circle[0].
static void GetCircleOffsets(int img_step, std::vector<int>& circle);

// computes the threshold table for marking the type of intensity difference.
// There're 255(minus)+1(0)+255(plus) elements in the table.
// for i = 0 to 511-1
//   if intensity < -threshold, thres_tab[i] = 1
//   if intensity > threshold, thres_tab[i] = 2
//   else thres_tab[i] = 0
// For example, the intensity difference is 100, threshold = 50, than it's 0x2.
static void GetThresTab(
    int threshold, std::vector<uchar>& thres_tab);

// computes FAST score
static int GetFastScore(
    const uchar* v, const std::vector<int>& circle);