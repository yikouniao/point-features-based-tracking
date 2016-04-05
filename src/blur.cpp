#include "blur.h"

void GaussianBlur(const Mat& src, Mat& dst, int kwidth, int kheight) {
  if (kwidth == 1 && kheight == 1) {
    dst = src;
    return;
  }

  Mat dst_(src.rows, src.cols);
  // dst_.allocate();

  // blur

  dst.Release();
  dst = dst_;
}