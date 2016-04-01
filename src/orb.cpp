#include "orb.h"
#include "fast.h"
#include "file.h"
#include <iostream>
#include <cmath>
#include <algorithm>

using namespace std;

const float harris_k = .04f;
size_t n_levels = 8;
float fx = 1.2f, fy = 1.2f;

void ORBTemp() {
  Mat img = ImgRead("../Homura.bmp");
  vector<Mat> pyramid;
  GetPyramid(img, pyramid, n_levels, fx, fy);
//  GetKeyPoints();
  img.Release();
}

static void GetKeyPoints(int nfeatures, int border_width, const std::vector<Rect>& layer_info, float scale_factor) {
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
  const Mat img(9, 9, m); /// put them in orb(), input pyramid

  vector<KeyPoint> keypoints;
  Mat mask;
  int fast_threshold{20};

  size_t nlevels = layer_info.size();
  vector<int> n_pts_per_level(nlevels);
  float factor = 1 / scale_factor;
  float ndesired_pts_per_level = nfeatures * (1 - factor) / (1 - (float)pow((double)factor, (double)nlevels));

  int sum_features = 0;
  for (size_t level = 0; level < nlevels - 1; level++) {
    n_pts_per_level[level] = lround(ndesired_pts_per_level);
    sum_features += n_pts_per_level[level];
    ndesired_pts_per_level *= factor;
  }
  n_pts_per_level[nlevels - 1] = max(nfeatures - sum_features, 0);

  for (size_t level = 0; level < nlevels; level++) {
    int featuresNum = n_pts_per_level[level];

    ///Mat img and mask per level

    // FAST detectors
    FastFeatureDetector* fd = new FastFeatureDetector(fast_threshold, true);
    fd->detect(img, keypoints, mask);
    delete fd;

    // Remove keypoints very close to the border
    KeyPointsFilterByImgBorder(keypoints, img, border_width);

    // Keep more points than necessary
    KeyPointsRetainBest(keypoints, n_pts_per_level[level] * 2);
    
    /// octave in FAST???
    /// Bilinear interpolation to resize image
  }

  mask.Release();
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
    // M = sum([Ix^2, IxIy; IxIy, Iy^2])
    // response = det(M) - k(trace(M))^2
    e.response = Ix_2 * Iy_2 - IxIy * IxIy - k * (Ix_2 + Iy_2) * (Ix_2 + Iy_2);
  }
}

static void GetPyramid(const Mat& img, std::vector<Mat>& pyramid,
                       size_t n_levels, float fx, float fy) {
  // Clear the pyramid
  for (size_t i = 0; i < pyramid.size(); ++i) {
    pyramid[i].Release();
  }
  pyramid.clear();

  // Construct the pyramid
  pyramid.push_back(img);
  for (size_t i = 1; i < n_levels; ++i) {
    pyramid.push_back(Resize(pyramid[i - 1], fx, fy));
  }
}