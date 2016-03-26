#include "file.h"
#include "err.h"

Mat ImgRead(std::string f_name) {
  std::string ext = GetFileExt(f_name);
  if (ext == "BMP") {
    BMP* img_file = new BMP();
    Mat img = img_file->Read(f_name);
    delete img_file;
    return img;
  } else {
    Err(f_name + " is not a bmp file!");
    return Mat();
  }
}

void ImgWrite(std::string f_name, const Mat& img) {
  std::string ext = GetFileExt(f_name);
  if (ext == "BMP") {
    BMP* img_file = new BMP();
    img_file->Write(f_name, img);
    delete img_file;
  } else {
    Err(f_name + " is not a bmp file!");
  }
}

std::string GetFileExt(const std::string& f_name) {
  std::string::size_type n = f_name.rfind('.');
  std::string ext;
  if (n != std::string::npos) {
    for (++n; n < f_name.length(); ++n) {
      ext += toupper(f_name[n]);
    }
  }
  return ext;
}