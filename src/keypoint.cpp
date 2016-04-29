#include "keypoint.h"
#include <algorithm>
#include <functional>

using namespace std;

KeyPoint::KeyPoint(const Pointf& point_, float size_, float angle_,
                   float response_, int octave_, int class_id_)
    : point(point_), size(size_), angle(angle_), response(response_),
      octave(octave_), class_id(class_id_) {}

KeyPoint::KeyPoint(float x_, float y_, float size_, float angle_,
                   float response_, int octave_, int class_id_)
    : point(x_, y_), size(size_), angle(angle_),
      response(response_), octave(octave_), class_id(class_id_) {}

KeyPoint::~KeyPoint() {}

bool operator >(const KeyPoint& a, const KeyPoint& b) {
  return a.response > b.response;
}

bool operator <(const KeyPoint& a, const KeyPoint& b) {
  return a.response < b.response;
}

bool operator ==(const KeyPoint& a, const KeyPoint& b) {
  return a.response == b.response;
}

bool operator !=(const KeyPoint& a, const KeyPoint& b) {
  return a.response != b.response;
}

void KeyPointsMask(std::vector<KeyPoint>& keypoints, const Mat& mask) {
  if (mask.Empty())
    return;

  for (auto it = keypoints.begin(); it != keypoints.end();) {
    mask(Point(it->point)) ? it = keypoints.erase(it) : ++it;
  }
}

void KeyPointsFilterByImgBorder(
    std::vector<KeyPoint>& keypoints, const Mat& img, size_t border_width) {
  for (auto& i = keypoints.begin(); i != keypoints.end();) {
    (i->point.x < border_width || i->point.x > img.cols - border_width - 1 ||
     i->point.y < border_width || i->point.y > img.rows - border_width - 1) ?
      i = keypoints.erase(i) : ++i;
  }
}

void KeyPointsRetainBest(std::vector<KeyPoint>& keypoints, size_t n_points) {
  if (n_points < keypoints.size()) {
    sort(keypoints.begin(), keypoints.end(), greater<KeyPoint>());
    keypoints.erase(keypoints.begin() + n_points, keypoints.end());
  }
}

void MarkKeyPoints(
    Mat& img, const std::vector<KeyPoint>& keypoints, int octave) {
  for (const auto& e : keypoints) {
    if (octave == -1 || octave == e.octave) {
      int y = lroundf(e.point.y);
      int x = lroundf(e.point.x);
      img(y, x - 1) = img(y, x) = img(y, x + 1) =
          img(y - 1, x) = img(y + 1, x) = img(y, x) > 180 ? 0 : 255;
    }
  }
}