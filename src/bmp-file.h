#pragma once

#include "types.h"
#include <string>

// considers only 8-bit gray .bmp file
class BMP {
public:
  static Mat Read(std::string f_name);
  static void Write(std::string f_name, const Mat& img);
};

struct BMPFileHeader {
  // img_size = rows * cols * bytes_per_pixel
  BMPFileHeader(size_t img_size = 0);
  // Exclude bf_type in order to read file header correctly.
  //ushort bf_type;
  uint bf_size;
  ushort bf_reserved1;
  ushort bf_reserved2;
  uint bf_off_bits;
};

struct BMPInfoHeader {
  // rows and cols are size of the image.
  // bit_count can be 8, 16, 24, 32...
  BMPInfoHeader(size_t rows = 0, size_t cols = 0, ushort bit_count = 8);
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
  uchar rgb_b;
  uchar rgb_g;
  uchar rgb_r;
  uchar rgb_reserved;
};