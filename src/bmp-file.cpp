#include "bmp-file.h"

Mat BMP::Read(std::string f_name) {
  FILE* fp;
  BMPFileHeader* bmp_file_header = new BMPFileHeader();
  BMPInfoHeader* bmp_info_header = new BMPInfoHeader();

  // Open the file
  errno_t err = fopen_s(&fp, f_name.c_str(), "rb");
  if (err != 0)
    throw("Cannot open file " + f_name + ".");

  if (fread(bmp_file_header, sizeof(BMPFileHeader), 1, fp) !=
        sizeof(BMPFileHeader))
    throw("Error in reading file header of " + f_name + ".");

  if (bmp_file_header->bf_type != 0x424D)
    throw(f_name + " is not a bmp file.");

  if (fread(&bmp_info_header, sizeof(BMPInfoHeader), 1, fp) !=
        sizeof(BMPInfoHeader))
    throw("Error in reading info header of " + f_name + ".");

  Mat img(bmp_info_header->bi_height, bmp_info_header->bi_width);

  if (!(img.data = (unsigned char *)malloc(img.rows * img.cols)))
    throw("Error in allocating memory.");

  // Read image data
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
    throw("Here consider 8-bit or 24-bit bmp only.");
  }

  fclose(fp);
  return img;
}

void BMP::Write(std::string f_name, const Mat& img) {

}