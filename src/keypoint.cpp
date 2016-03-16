#include "keypoint.h"
#include <algorithm>
#include <functional>

bool operator>(const KeyPoint& a, const KeyPoint& b)
{
  return a.response > b.response;
}

void KeyPointsMask(std::vector<KeyPoint>& keypoints, const Mat& mask) {
  if (mask.empty())
    return;

  for (auto i = keypoints.begin(); i != keypoints.end();) {
    mask((size_t)(i->y + .5f), (size_t)(i->x + .5f)) ?
      i = keypoints.erase(i) : ++i;
  }
}

void KeyPointsFilterByImgBorder(std::vector<KeyPoint>& keypoints,
                                const Mat& img, size_t border_width) {
  for (auto& i = keypoints.begin(); i != keypoints.end();) {
    (i->x < border_width || i->x > img.cols - border_width - 1 ||
     i->y < border_width || i->y > img.rows - border_width - 1) ?
      i = keypoints.erase(i) : ++i;
  }
}

void KeyPointsRetainBest(std::vector<KeyPoint>& keypoints, size_t n_points) {
  std::sort(keypoints.begin(), keypoints.end(), std::greater<KeyPoint>());
  keypoints.erase(keypoints.begin() + n_points, keypoints.end());
}
