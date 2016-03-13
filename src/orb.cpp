#include "orb.h"
#include "fast.h"

using namespace cv;

void ORB() {
  //cv::Mat img = cv::imread("Witch_Yoshino.png");
  //cv::cvtColor(img, img, CV_RGB2GRAY);

  unsigned char m[9][9] = { 
    {  50,  50,  50,  50,  50,  50,  50,  50,  50 },
    {  50,  50,  50, 222, 222,  50,  50,  50,  50 },
    {  50,  50, 222,  50,  50,  50, 222,  50,  50 },
    {  50,   0,  50,  50,  50,  50,  50, 222,  50 },
    {  50,   0,  50,  50,  50,  50,  50, 222,  50 },
    {  50,   0,  50,  50,  50,  50,  50,   0,  50 },
    {  50,  50,   0,  50,  50,  50,   0,  50,  50 },
    {  50,  50,  50,   0,   0,   0,  50,  50,  50 },
    {  50,  50,  50,  50,  50,  50,  50,  50,  50 }};
  Mat img = Mat(9, 9, CV_8U, m);

  std::vector<cv::KeyPoint> keypoints;
  cv::Mat mask;
  int fastThreshold(20);

#if 1
  FastFeatureDetectorT* fd = new FastFeatureDetectorT(fastThreshold, true);
  fd->detect(img, keypoints, mask);
#else
  Ptr<FastFeatureDetector> fd = FastFeatureDetector::create(fastThreshold, true);
  fd->detect(img, keypoints, mask);
#endif
}