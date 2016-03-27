#pragma once

class Rect {
 public:
  // constructors
  Rect(int x_ = 0, int y_ = 0, int width_ = 0, int height_ = 0);

  // destructors
  ~Rect();

  int x, y, width, height;
};