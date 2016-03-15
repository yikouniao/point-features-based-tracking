#pragma once

class KeyPoint {
 public:
  KeyPoint(float x_, float y_, float diameter_, float angle_ = -1, float response_ = 0, int octave_ = 0, int class_id_ = -1)
      : x(x_), y(y_), diameter(diameter_), angle(angle_), response(response_), octave(octave_), class_id(class_id) {}
  float x, y; // keypoint coordinates
  float diameter; // keypoint diameter
  float angle; // keypoint orientation (-1 if not applicable)
  float response; // keypoint detector response on the keypoint
  int octave; // octave in pyramid layers
  int class_id; // object class (-1 if not applicable)
};