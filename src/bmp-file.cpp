#include "bmp-file.h"
#include "err.h"

Mat BMP::Read(std::string f_name) {
  FILE* fp;
  BMPFileHeader* bmp_file_header = new BMPFileHeader();
  BMPInfoHeader* bmp_info_header = new BMPInfoHeader();

  // opens the file
  if (errno_t err = fopen_s(&fp, f_name.c_str(), "rb"))
    Err("Cannot open file " + f_name + ".");

  ushort bf_type;
  fread(&bf_type, 1, sizeof(ushort), fp);
  if (0x4D42 != bf_type)
    Err(f_name + " is not a bmp file.");
  
  fread(bmp_file_header, sizeof(BMPFileHeader), 1, fp);
  fread(bmp_info_header, sizeof(BMPInfoHeader), 1, fp);

  Mat img(bmp_info_header->bi_height, bmp_info_header->bi_width);

  if (!(img.data = new (std::nothrow) uchar[img.rows * img.cols]))
    Err("Error in allocating memory for image data.");

  // reads image data
  if (bmp_info_header->bi_bit_count == 8) {
    size_t linebytes = ((img.cols + 3) / 4) * 4;
    for (size_t i = 0; i < img.rows; ++i) {
      int off = linebytes * (i + 1);
      fseek(fp, -off, SEEK_END);
      fread(&img(i, 0), 1, img.cols, fp);
    }
  } else if (bmp_info_header->bi_bit_count == 24) {
    size_t linebytes = ((img.cols * 3 + 3) / 4) * 4;
    for (size_t i = 0; i < img.rows; ++i) {
      int off = linebytes * (i + 1);
      fseek(fp, -off, SEEK_END);
      for (size_t j = 0; j < img.cols; ++j) {
        uchar d1, d2, d3;
        fread(&d1, 1, 1, fp); fread(&d2, 1, 1, fp); fread(&d3, 1, 1, fp);
        img(i, j) = (uchar)((d1 * 299 + d2 * 587 + d3 * 114) / 1000);
      }
    }
  } else {
    Err("Here consider only 8-bit or 24-bit bmp.");
  }

  delete bmp_file_header;
  delete bmp_info_header;
  fclose(fp);
  return img;
}

void BMP::Write(std::string f_name, const Mat& img) {
  FILE* fp;
  BMPFileHeader* bmp_file_header = new BMPFileHeader(img.rows * img.cols);
  BMPInfoHeader* bmp_info_header = new BMPInfoHeader(img.rows, img.cols);
  RGBQuad* bmi_colors = new (std::nothrow) RGBQuad[256];
  if (!bmi_colors)
    Err("Error in allocating memory for RGBQuad.");

  for (size_t i = 0; i < 256; ++i) {
    bmi_colors[i].rgb_r = bmi_colors[i].rgb_g = bmi_colors[i].rgb_b = (uchar)i;
    bmi_colors[i].rgb_reserved = 0;
  }

  errno_t err = fopen_s(&fp, f_name.c_str(), "wb");
  if (err != 0)
    Err("Cannot open file " + f_name + ".");
  
  ushort bf_type = 0x4D42;
  fwrite(&bf_type, sizeof(bf_type), 1, fp);
  fwrite(bmp_file_header, sizeof(BMPFileHeader), 1, fp);
  fwrite(bmp_info_header, sizeof(BMPInfoHeader), 1, fp);
  fwrite(bmi_colors, sizeof(RGBQuad), 256, fp);
  
  size_t linebytes = ((img.cols + 3) / 4) * 4;
  size_t skipstep = linebytes - img.cols;
  uchar skip[3] = {0};

  for (int i = img.rows - 1; i >= 0; --i) {
    fwrite(&img(i, 0), 1, img.cols, fp);
    fwrite(skip, 1, skipstep, fp);
  }

  delete bmp_file_header;
  delete bmp_info_header;
  delete[] bmi_colors;
  fclose(fp);
}

BMPFileHeader::BMPFileHeader(size_t img_size) :
    bf_reserved1(0), bf_reserved2(0), bf_off_bits(54 + 256 * 4) {
  bf_size = img_size + bf_off_bits;
}

BMPInfoHeader::BMPInfoHeader(size_t rows, size_t cols, ushort bit_count) :
    bi_size(40), bi_width(cols), bi_height(rows), bi_planes(1),
    bi_bit_count(bit_count), bi_compression(0), bi_xpels_per_meter(0),
    bi_ypels_per_meter(0), bi_clr_used(0), bi_clr_important(0) {
  bi_size_image = rows * cols * bit_count / 8;
}