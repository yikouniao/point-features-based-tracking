#include "keypoint.h"
#include <algorithm>
#include <functional>

using namespace std;

KeyPoint::KeyPoint(Pointf pt_, float diameter_, float angle_,
                   float response_, int octave_, int class_id_)
    : Pointf(pt_), diameter(diameter_), angle(angle_),
      response(response_), octave(octave_), class_id(class_id_) {}

KeyPoint::KeyPoint(float x_, float y_, float diameter_, float angle_,
                   float response_, int octave_, int class_id_)
    : Pointf(x_, y_), diameter(diameter_), angle(angle_),
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

  for (auto i = keypoints.begin(); i != keypoints.end();) {
    mask(lround(i->y), lround(i->x)) ? i = keypoints.erase(i) : ++i;
  }
}

void KeyPointsFilterByImgBorder(
    std::vector<KeyPoint>& keypoints, const Mat& img, size_t border_width) {
  for (auto& i = keypoints.begin(); i != keypoints.end();) {
    (i->x < border_width || i->x > img.cols - border_width - 1 ||
     i->y < border_width || i->y > img.rows - border_width - 1) ?
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
    std::vector<Mat>& img_pyramid, const std::vector<KeyPoint>& keypoints) {
  for (size_t i = 0; i < img_pyramid.size(); ++i) {
    MarkKeyPoints(img_pyramid[i], keypoints, i);
  }
}

void MarkKeyPoints(
    Mat& img, const std::vector<KeyPoint>& keypoints, int octave) {
  for (const auto& e : keypoints) {
    if (octave == -1 || octave == e.octave) {
      int y = lroundf(e.y);
      int x = lroundf(e.x);
      img(y, x - 1) = img(y, x) = img(y, x + 1) =
          img(y - 1, x) = img(y + 1, x) = 255;
    }
  }
}