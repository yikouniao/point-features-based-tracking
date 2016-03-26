#pragma once

#include "types.h"
#include <string>

// Consider 8-bit gray .bmp file only.
class BMP {
public:
  static Mat Read(std::string f_name);
  static void Write(std::string f_name, const Mat& img);
};

struct BMPFileHeader {
  ushort bf_type;
  uint bf_size;
  ushort bf_reserved1;
  ushort bf_reserved2;
  uint bf_offBits;
};

struct BMPInfoHeader {
  uint bi_size;
  int bi_width;
  int bi_height;
  ushort bi_planes;
  ushort bi_bit_count;
  uint bi_compression;
  uint bi_size_image;
  int bi_xpels_per_meter;
  int bi_ypels_per_meter;
  uint bi_clr_used;
  uint bi_clr_important;
};

struct RGBQuad {
  uchar rgb_blue;
  uchar rgb_green;
  uchar rgb_red;
  uchar rgb_reserved;
};