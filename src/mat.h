#pragma once

#include "rect.h"

// two-dimensional matrix, type of data is unsigned char
class Mat {
 public:
  // constructors
  Mat();
  Mat(size_t rows_, size_t cols_, void* data_ = nullptr);
  Mat(size_t rows_, size_t cols_, size_t step_, void* data_ = nullptr);
  Mat(const Mat& m);
  Mat(size_t rows_, size_t cols_, unsigned char init_v);
  // destructor
  // Will not deallocate memory. Call Release() to deallocate memory
  // if necessary.
  ~Mat();

  void Release(); // Deallocate memory

  bool empty() const; // Return true if data == nullptr

  // Return reference to a specified element.
  unsigned char& operator ()(int i, int j);
  // Return const reference to a specified element.
  const unsigned char& operator ()(int i, int j) const;
  Mat operator ()(const Rect& roi) const;
  Mat& operator =(const Mat& m); // assignment operator, shallow copy

  // Release old memory, allocate new memory and assign the new contents.
  void Clone(const Mat& m);
  void Creat(size_t rows_, size_t cols_, unsigned char init_v);

  size_t rows, cols; // the size of the matrix
  // quantity of uchar from a pixel to next-row one. When the data
  // is a rectangle part of other data, step may be unequal to cols.
  size_t step;
  unsigned char* data; // the contents of the matrix
};