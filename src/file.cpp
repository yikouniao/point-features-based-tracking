#include "file.h"

Mat ImgRead(std::string f_name) {
  std::string ext_ = GetFileExt(f_name);
  std::string ext(0);
  for (size_t i = 0; i < ext_.length(); ++i) {
    ext += toupper(ext_[i]);
  }

  if (ext == "BMP") {
    BMP* img_file = new BMP();
    return img_file->Read(f_name);
  } else {
    throw(f_name + " is not a bmp file!");
  }
}

std::string GetFileExt(const std::string& f_name) {
  std::string::size_type n = f_name.rfind('.');
  return f_name.substr(n == std::string::npos ? f_name.length() : n + 1);
}