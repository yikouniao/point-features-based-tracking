#pragma once

// two-dimensional matrix, type of data is unsigned char
class Mat {
 public:
  Mat(size_t rows_, size_t cols_, void* data_ = nullptr)
      : rows(rows_), cols(cols_), data((unsigned char**)data_) {}
  Mat() : rows(0), cols(0), data(nullptr) {}
  bool empty() const; // return true if data == nullptr
  size_t rows, cols; // the size of the matrix
  unsigned char** data; // the contents of the matrix
};