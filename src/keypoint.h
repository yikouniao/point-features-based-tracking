#pragma once

#include "point.h"
#include <vector>
#include "mat.h"

class KeyPoint : public Pointf {
 public:
  // constructors
  KeyPoint(Pointf pt_, float diameter_, float angle_ = -1,
           float response_ = 0, int octave_ = -1, int class_id_ = -1);
  KeyPoint(float x_, float y_, float diameter_, float angle_ = -1,
           float response_ = 0, int octave_ = -1, int class_id_ = -1);

  // destructor
  ~KeyPoint();

  float diameter; // keypoint diameter
  float angle; // keypoint orientation (-1 if not applicable)
  float response; // keypoint detector response on the keypoint
  int octave; // octave in pyramid layers (-1 if not applicable)
  int class_id; // object class (-1 if not applicable)
};

// compares responses
bool operator >(const KeyPoint& a, const KeyPoint& b);
bool operator <(const KeyPoint& a, const KeyPoint& b);
bool operator ==(const KeyPoint& a, const KeyPoint& b);
bool operator !=(const KeyPoint& a, const KeyPoint& b);

// discards points in the mask
void KeyPointsMask(std::vector<KeyPoint>& keypoints, const Mat& mask);

// discards points near the image border
void KeyPointsFilterByImgBorder(
    std::vector<KeyPoint>& keypoints, const Mat& img, size_t border_width);

// retains top n points by responses
void KeyPointsRetainBest(std::vector<KeyPoint>& keypoints, size_t n_points);

// marks keypoints with a white cross on an image pyramid
void MarkKeyPoints(
    std::vector<Mat>& img_pyramid, const std::vector<KeyPoint>& keypoints);

// marks keypoints with a white cross on an image
// If octave = -1, mark all keypoints. Else mark specific keypoints.
void MarkKeyPoints(
    Mat& img, const std::vector<KeyPoint>& keypoints, int octave = -1);