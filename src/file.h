#pragma once

#include "bmp-file.h"
#include <string>
#include "mat.h"

// Read a image file
// Consider 8-bit gray .bmp file only.
Mat ImgRead(std::string f_name);

// Get file extension.
std::string GetFileExt(const std::string& f_name);