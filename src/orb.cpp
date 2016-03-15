#include "orb.h"
#include "fast.h"
#include <iostream>

void ORB() {
  unsigned char m[][9] = { 
    {  50,  50,  50,  50,  50,  50,  50,  50,  50 },
    {  50,  50,  50, 222, 222,  50,  50,  50,  50 },
    {  50,  50, 222,  50,  50,  50, 222,  50,  50 },
    {  50,   0,  50,  50,  50,  50,  50, 222,  50 },
    {  50,   0,  50,  50,  48,  50,  50, 222,  50 },
    {  50,   0,  50,  50,  50,  50,  50,   0,  50 },
    {  50,  50,   0,  50,  50,  50,   0,  50,  50 },
    {  50,  50,  50,   0,   0,   0,  50,  50,  50 },
    {  50,  50,  50,  50,  50,  50,  50,  50,  50 }};
  const Mat img(9, 9, m);

  std::vector<KeyPoint> keypoints;
  Mat mask;
  int fast_threshold{20};

  // FAST detectors
  FastFeatureDetector* fd = new FastFeatureDetector(fast_threshold, true);
  fd->detect(img, keypoints, mask);
}