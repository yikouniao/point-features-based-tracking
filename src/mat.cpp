#include "mat.h"

Mat::~Mat() {}

void Mat::Release() {
  delete[] data;
}

bool Mat::empty() const {
  return data == nullptr;
}

unsigned char& Mat::operator() (size_t i, size_t j) {
  return data[i * step + j];
}

const unsigned char& Mat::operator() (size_t i, size_t j) const {
  return ((const unsigned char*)data)[i * step + j];
}

Mat Mat::operator()(const Rect& roi) const {
  return Mat(roi.height, roi.width, step, &Mat(roi.y, roi.x));
}

Mat& Mat::operator= (const Mat& m) {
  rows = m.rows, cols = m.cols, step = m.step;
  data = m.data;
  return *this;
}