#include "mat.h"

Mat::Mat(size_t rows_, size_t cols_, size_t step_, void* data_)
    : rows(rows_), cols(cols_), step(step_), data((unsigned char*)data_) {}

Mat::Mat(size_t rows_, size_t cols_, void* data_)
    : rows(rows_), cols(cols_), step(cols_), data((unsigned char*)data_) {}

Mat::Mat(const Mat& m) {
  *this = m;
}

Mat::Mat() : rows(0), cols(0), step(0), data(nullptr) {}

Mat::~Mat() {}

void Mat::Release() {
  delete[] data;
  data = nullptr;
  rows = cols = step = 0;
}

bool Mat::empty() const {
  return data == nullptr;
}

unsigned char& Mat::operator ()(int i, int j) {
  return data[i * step + j];
}

const unsigned char& Mat::operator ()(int i, int j) const {
  return ((const unsigned char*)data)[i * step + j];
}

Mat Mat::operator ()(const Rect& roi) const {
  return Mat(roi.height, roi.width, step, &Mat(roi.y, roi.x));
}

Mat& Mat::operator =(const Mat& m) {
  rows = m.rows, cols = m.cols, step = m.step;
  data = m.data;
  return *this;
}