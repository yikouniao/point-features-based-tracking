#include "orb.h"
#include "fast.h"
#include <iostream>

const float harris_k = .04f;

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

  int nfeatures = 500;
  int border_width = 31;

  std::vector<KeyPoint> keypoints;
  Mat mask;
  int fast_threshold{20};

  // FAST detectors
  FastFeatureDetector* fd = new FastFeatureDetector(fast_threshold, true);
  fd->detect(img, keypoints, mask);

  // Remove keypoints very close to the border
  KeyPointsFilterByImgBorder(keypoints, img, border_width);

  // Keep more points than necessary
  size_t n_points = nfeatures / 2; // temp
  KeyPointsRetainBest(keypoints, n_points * 2);
  
  // octave in FAST???
}

static void HarrisResponses(
    std::vector<KeyPoint>& keypoints, const Mat& img,
    const std::vector<Rect>& layer_info, size_t block_size, float k) {
  if (img.rows < block_size || img.cols < block_size)
    return;

  int Ix_2 = 0 /* Ix*Ix */, Iy_2 = 0 /* Iy*Iy */, IxIy = 0 /* Ix*Iy */;
  for (auto& e : keypoints) {
    for (size_t i = 0; i < block_size; ++i) {
      for (size_t j = 0; j < block_size; ++j) {
        int y0 = (int)e.y - block_size / 2 + layer_info[e.octave].y;
        int x0 = (int)e.x - block_size / 2 + layer_info[e.octave].x;
        // x gradient, Sobel
        int Ix = img(y0 - 1, x0 + 1) - img(y0 - 1, x0 - 1) +
                 (img(y0, x0 + 1) - img(y0, x0 - 1)) * 2 +
                 img(y0 + 1, x0 + 1) - img(y0 + 1, x0 - 1);
        // y gradient, Sobel
        int Iy = img(y0 + 1, x0 - 1) - img(y0 - 1, x0 - 1) +
                 (img(y0 + 1, x0) - img(y0 - 1, x0)) * 2 +
                 img(y0 + 1, x0 + 1) - img(y0 - 1, x0 + 1);
        Ix_2 += Ix * Ix;
        Iy_2 += Iy * Iy;
        IxIy += Ix * Iy;
      }
    }
    e.response = Ix_2 * Iy_2 - IxIy * IxIy - k * (Ix_2 + Iy_2) * (Ix_2 + Iy_2);
  }
}