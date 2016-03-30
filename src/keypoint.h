#pragma once

#include "point.h"
#include <vector>
#include "mat.h"

class KeyPoint : public Pointf {
 public:
  // constructors
  KeyPoint(Pointf pt_, float diameter_, float angle_ = -1,
           float response_ = 0, int octave_ = 0, int class_id_ = -1);
  KeyPoint(float x_, float y_, float diameter_, float angle_ = -1,
           float response_ = 0, int octave_ = 0, int class_id_ = -1);

  // destructor
  ~KeyPoint();

  float diameter; // keypoint diameter
  float angle; // keypoint orientation (-1 if not applicable)
  float response; // keypoint detector response on the keypoint
  int octave; // octave in pyramid layers
  int class_id; // object class (-1 if not applicable)
};

// compares responses
bool operator >(const KeyPoint& a, const KeyPoint& b);
bool operator <(const KeyPoint& a, const KeyPoint& b);
bool operator ==(const KeyPoint& a, const KeyPoint& b);
bool operator !=(const KeyPoint& a, const KeyPoint& b);

void KeyPointsMask(std::vector<KeyPoint>& keypoints, const Mat& mask);
void KeyPointsFilterByImgBorder(std::vector<KeyPoint>& keypoints,
                                const Mat& img, size_t border_width);
void KeyPointsRetainBest(std::vector<KeyPoint>& keypoints, size_t n_points);