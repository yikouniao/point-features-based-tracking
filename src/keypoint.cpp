#include "keypoint.h"
#include <algorithm>
#include <functional>

KeyPoint::KeyPoint(float x_, float y_, float diameter_, float angle_,
                   float response_, int octave_, int class_id_)
    : x(x_), y(y_), diameter(diameter_), angle(angle_),
      response(response_), octave(octave_), class_id(class_id_) {}

KeyPoint::~KeyPoint() {}

bool operator >(const KeyPoint& a, const KeyPoint& b) {
  return a.response > b.response;
}

void KeyPointsMask(std::vector<KeyPoint>& keypoints, const Mat& mask) {
  if (mask.Empty())
    return;

  for (auto i = keypoints.begin(); i != keypoints.end();) {
    mask(lround(i->y), lround(i->x)) ? i = keypoints.erase(i) : ++i;
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
  if (n_points > keypoints.size()) {
    std::sort(keypoints.begin(), keypoints.end(), std::greater<KeyPoint>());
    keypoints.erase(keypoints.begin() + n_points, keypoints.end());
  }
}
