#pragma once

#include "img-file.h"

// Consider 8-bit gray .bmp file only.
class BMP : public ImgFile {
public:
  static Mat Read(std::string f_name);
  static void Write(std::string f_name, const Mat& img);
};