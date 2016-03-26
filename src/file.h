#pragma once

#include "bmp-file.h"
#include <string>
#include "types.h"

// Read and write a image file
// Consider 8-bit gray .bmp file only.
Mat ImgRead(std::string f_name);
void ImgWrite(std::string f_name, const Mat& img);

// Get file extension and return its upper style.
std::string GetFileExt(const std::string& f_name);