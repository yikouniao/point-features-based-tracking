#pragma once

#include "mat.h"
#include <string>

class ImgFile {
public:
  virtual Mat Read(std::string f_name) const = 0;
  virtual void Write(std::string f_name, const Mat& img) const = 0;
};