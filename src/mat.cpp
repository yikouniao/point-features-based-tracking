#include "mat.h"
#include "err.h"

Mat::Mat() : rows(0), cols(0), step(0), data(nullptr) {}

Mat::Mat(size_t rows_, size_t cols_, void* data_)
    : rows(rows_), cols(cols_), step(cols_), data((unsigned char*)data_) {}

Mat::Mat(size_t rows_, size_t cols_, size_t step_, void* data_)
    : rows(rows_), cols(cols_), step(step_), data((unsigned char*)data_) {}

Mat::Mat(const Mat& m) {
  *this = m;
}

Mat::Mat(size_t rows_, size_t cols_, unsigned char init_v)
    : rows(rows_), cols(cols_), step(cols_) {
  if (!(data = new unsigned char[rows * cols]{init_v}))
    Err("Error in allocating memory for Mat.data in the constructor.");
}

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
  if (roi.x < 0 || roi.y < 0 || roi.width < 0 || roi.height < 0 ||
      size_t(roi.x + roi.width) > cols || size_t(roi.x + roi.height) > rows)
    return *this;
  return Mat(roi.height, roi.width, step, &Mat(roi.y, roi.x));
}

Mat& Mat::operator =(const Mat& m) {
  rows = m.rows, cols = m.cols, step = m.step;
  data = m.data;
  return *this;
}

void Mat::Clone(const Mat& m) {
  Release();
  rows = m.rows, cols = m.cols, step = m.cols;
  if (!(data = new unsigned char[rows * cols]))
    Err("Error in allocating memory for Mat.data while cloning.");
  for (size_t i = 0; i < rows; ++i) {
    for (size_t j = 0; j < cols; ++j) {
      (*this)(i, j) = m(i, j);
    }
  }
}

void Mat::Creat(size_t rows_, size_t cols_, unsigned char init_v) {
  Release();
  rows = rows_, cols = cols_, step = cols_;
  if (!(data = new unsigned char[rows * cols]{ init_v }))
    Err("Error in allocating memory for Mat.data in the constructor.");
}
