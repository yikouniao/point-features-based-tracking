#pragma once

class Rect {
 public:
  Rect(int x_ = 0, int y_ = 0, int width_ = 0, int height_ = 0)
      : x(x_), y(y_), width(width_), height(height_) {}

  int x, y, width, height;
};