#pragma once

#include "rect.h"

// two-dimensional matrix, type of data is unsigned char
class Mat {
 public:
  Mat(size_t rows_, size_t cols_, size_t step_, void* data_ = nullptr)
      : rows(rows_), cols(cols_), step(step_), data((unsigned char*)data_) {}
  Mat(size_t rows_, size_t cols_, void* data_ = nullptr)
      : rows(rows_), cols(cols_), step(cols_), data((unsigned char*)data_) {}
  Mat() : rows(0), cols(0), step(0), data(nullptr) {}
  ~Mat();

  // returns reference to a specified element
  unsigned char& operator() (size_t i, size_t j);
  // returns const reference to a specified element
  const unsigned char& operator() (size_t i, size_t j) const;
  Mat Mat::operator()(const Rect& roi) const;
  Mat& operator= (const Mat& m); // assignment operator

  bool empty() const; // return true if data == nullptr

  size_t rows, cols; // the size of the matrix
  // quantity of uchar from a pixel to next-row one. When the data
  // is a rectangle part of other data, step may be unequal to cols.
  size_t step;
  unsigned char* data; // the contents of the matrix
};