#pragma once

#include "mat.h"
#include <string>

// Consider 8-bit gray .bmp file only.
class BMP {
public:
  static Mat Read(std::string f_name);
  static void Write(std::string f_name, const Mat& img);
};