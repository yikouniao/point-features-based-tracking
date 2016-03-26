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

  if (bmp_file_header->bf_type != 0x4D42)
    throw(f_name + " is not a bmp file.");

  if (fread(&bmp_info_header, sizeof(BMPInfoHeader), 1, fp) !=
        sizeof(BMPInfoHeader))
    throw("Error in reading info header of " + f_name + ".");

  Mat img(bmp_info_header->bi_height, bmp_info_header->bi_width);

  if (!(img.data = new uchar[img.rows * img.cols]))
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

  delete bmp_file_header;
  delete bmp_info_header;
  fclose(fp);
  return img;
}

void BMP::Write(std::string f_name, const Mat& img) {
  FILE* fp;
  BMPFileHeader* bmp_file_header = new BMPFileHeader();
  BMPInfoHeader* bmp_info_header = new BMPInfoHeader();
  RGBQuad* bmi_colors = new RGBQuad[256];

  bmp_file_header->bf_type = (short)0x4D42;
  bmp_file_header->bf_reserved1 = 0;
  bmp_file_header->bf_reserved2 = 0;

  bmp_info_header->bi_size = 40;
  bmp_info_header->bi_planes = 1;
  bmp_info_header->bi_compression = 0;
  bmp_info_header->bi_xpels_per_meter = 0;
  bmp_info_header->bi_ypels_per_meter = 0;
  bmp_info_header->bi_clr_used = 0;
  bmp_info_header->bi_clr_important = 0;

  for (size_t i = 0; i<256; i++) {
    bmi_colors[i].rgb_red = (uchar)i;
    bmi_colors[i].rgb_green = (uchar)i;
    bmi_colors[i].rgb_blue = (uchar)i;
    bmi_colors[i].rgb_reserved = 0;
  }

  errno_t err = fopen_s(&fp, f_name.c_str(), "wb");
  if (err != 0)
    throw("Cannot open file " + f_name + ".");

  size_t linebytes = ((img.cols + 3) / 4) * 4;
  //skipstep = linebytes - Col;

  //FileHeader.bfSize = Row*Col + 54 + 256 * 4;
  //FileHeader.bfOffBits = 54 + 256 * 4;

  //InfoHeader.biWidth = Col;
  //InfoHeader.biHeight = Row;
  //InfoHeader.biBitCount = 8;
  //InfoHeader.biSizeImage = Row*Col;


  ////fwrite(&FileHeader,sizeof(BITMAPFILEHEADER),1,fp);
  //fwrite(&FileHeader.bfType, 1, sizeof(unsigned short), fp);
  //fwrite(&FileHeader.bfSize, 1, sizeof(unsigned long), fp);
  //fwrite(&FileHeader.bfReserved1, 1, sizeof(unsigned short), fp);
  //fwrite(&FileHeader.bfReserved2, 1, sizeof(unsigned short), fp);
  //fwrite(&FileHeader.bfOffBits, 1, sizeof(unsigned long), fp);

  ////	fwrite(&FileHeader,14,1,fp);
  //fwrite(&InfoHeader, sizeof(_BITMAPINFOHEADER), 1, fp);
  //fwrite(bmiColors, sizeof(_RGBQUAD), 256, fp);

  //for (i = Row - 1; i >= 0; i--)
  //{
  //  off = (long)(Col)*(long)(i + 1);
  //  fwrite(image[i], 1, Col, fp);
  //  fwrite(skip, 1, skipstep, fp);
  //}
  //delete bmp_file_header;
  //delete bmp_info_header;
  //delete[] bmi_colors;
  //fclose(fp);

  //return 1;
}