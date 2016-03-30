#pragma once

#include "bmp-file.h"
#include <string>
#include "types.h"

// reads and writes a image file
// considers only 8-bit gray .bmp file
Mat ImgRead(std::string f_name);
void ImgWrite(std::string f_name, const Mat& img);

// gets file extension and return its upper style.
std::string GetFileExt(const std::string& f_name);