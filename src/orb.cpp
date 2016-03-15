#include "orb.h"
#include "fast.h"

void ORB() {
  //cv::Mat img = cv::imread("Witch_Yoshino.png");
  //cv::cvtColor(img, img, CV_RGB2GRAY);

  unsigned char m[][9] = { 
    {  50,  50,  50,  50,  50,  50,  50,  50,  50 },
    {  50,  50,  50, 222, 222,  50,  50,  50,  50 },
    {  50,  50, 222,  50,  50,  50, 222,  50,  50 },
    {  50,   0,  50,  50,  50,  50,  50, 222,  50 },
    {  50,   0,  50,  50,  50,  50,  50, 222,  50 },
    {  50,   0,  50,  50,  50,  50,  50,   0,  50 },
    {  50,  50,   0,  50,  50,  50,   0,  50,  50 },
    {  50,  50,  50,   0,   0,   0,  50,  50,  50 },
    {  50,  50,  50,  50,  50,  50,  50,  50,  50 }};
  Mat img = Mat(9, 9, m);
  
  std::vector<KeyPoint> keypoints;
  Mat mask;
  int fastThreshold{20};

  FastFeatureDetector* fd = new FastFeatureDetector(fastThreshold, true);
  fd->detect(img, keypoints, mask);
}