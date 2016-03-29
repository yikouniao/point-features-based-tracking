#pragma once

#include "rect.h"
#include "err.h"

// two-dimensional matrix
template<typename T>
class Mat_ {
 public:
  // constructors
  Mat_();
  Mat_(size_t rows_, size_t cols_, void* data_ = nullptr);
  Mat_(size_t rows_, size_t cols_, size_t step_, void* data_ = nullptr);
  Mat_(const Mat_& m);
  Mat_(size_t rows_, size_t cols_, T init_v);
  template<size_t C>
  Mat_(T a[][C], size_t rows_);
  // destructor
  // It will not deallocate memory. Call Release() to deallocate memory
  // if necessary.
  ~Mat_();

  void Release(); // Deallocate memory and reset all elements in the class.

  bool Empty() const; // Return true if data == nullptr

  // Release old memory, allocate new memory and assign the new contents.
  void Clone(const Mat_& m);
  void Creat(size_t rows_, size_t cols_, T init_v);

  // Return reference to a specified element.
  T& operator ()(int i, int j);
  // Return const reference to a specified element.
  const T& operator ()(int i, int j) const;
  Mat_<T> operator ()(const Rect_<T>& roi) const;
  Mat_<T>& operator =(const Mat_& m); // assignment operator, shallow copy

  Mat_<T>& operator -();
  template<typename Tm>
  Mat_<T>& operator +=(const Mat_<Tm>& m);
  template<typename Tm>
  Mat_<T>& operator -=(const Mat_<Tm>& m);
  //Mat_<T>& operator *=(double a);

  size_t rows, cols; // the size of the matrix
  // quantity of elements from a pixel to next-row one. When the data
  // is a rectangle part of other data, step may be unequal to cols.
  size_t step;
  T* data; // the contents of the matrix
};

using Matuc = Mat_<unsigned char>;
using Mati = Mat_<int>;
using Matf = Mat_<float>;
using Matd = Mat_<double>;
using Mat = Matuc;

template<typename T>
Mat_<T>::Mat_() : rows(0), cols(0), step(0), data(nullptr) {}

template<typename T>
Mat_<T>::Mat_(size_t rows_, size_t cols_, void* data_)
    : rows(rows_), cols(cols_), step(cols_), data((T*)data_) {}

template<typename T>
Mat_<T>::Mat_(size_t rows_, size_t cols_, size_t step_, void* data_)
    : rows(rows_), cols(cols_), step(step_), data((T*)data_) {}

template<typename T>
Mat_<T>::Mat_(const Mat_& m) {
  *this = m;
}

template<typename T>
Mat_<T>::Mat_(size_t rows_, size_t cols_, T init_v)
    : rows(rows_), cols(cols_), step(cols_) {
  if (!(data = new T[rows * cols]{init_v}))
    Err("Error in allocating memory for Mat_<T>::data in the constructor.");
}

template<typename T>
template<size_t C>
Mat_<T>::Mat_(T a[][C], size_t rows_) {
  rows = rows_; cols = C; step = cols;
  if (!(data = new T[rows * cols]))
    Err("Error in allocating memory for Mat_<T>::data in the constructor.");
  for (size_t i = 0; i < rows; ++i) {
    for (size_t j = 0; j < cols; ++j) {
      (*this)(i, j) = a[i][j];
    }
  }
}

template<typename T>
Mat_<T>::~Mat_() {}

template<typename T>
void Mat_<T>::Release() {
  delete[] data;
  data = nullptr;
  rows = cols = step = 0;
}

template<typename T>
bool Mat_<T>::Empty() const {
  return data == nullptr;
}

template<typename T>
void Mat_<T>::Clone(const Mat_<T>& m) {
  Release();
  rows = m.rows, cols = m.cols, step = m.cols;
  if (!(data = new T[rows * cols]))
    Err("Error in allocating memory for Mat_<T>::data while cloning.");
  for (size_t i = 0; i < rows; ++i) {
    for (size_t j = 0; j < cols; ++j) {
      (*this)(i, j) = m(i, j);
    }
  }
}

template<typename T>
void Mat_<T>::Creat(size_t rows_, size_t cols_, T init_v) {
  Release();
  rows = rows_, cols = cols_, step = cols_;
  if (!(data = new T[rows * cols]{ init_v }))
    Err("Error in allocating memory for Mat_<T>::data in the constructor.");
}

template<typename T>
T& Mat_<T>::operator ()(int i, int j) {
  return data[i * step + j];
}

template<typename T>
const T& Mat_<T>::operator ()(int i, int j) const {
  return ((const T*)data)[i * step + j];
}

template<typename T>
Mat_<T> Mat_<T>::operator ()(const Rect_<T>& roi) const {
  if (roi.x < 0 || roi.y < 0 || roi.width < 0 || roi.height < 0 ||
    size_t(roi.x + roi.width) > cols || size_t(roi.x + roi.height) > rows)
    return *this;
  return Mat_<T>(roi.height, roi.width, step, &Mat_<T>(roi.y, roi.x));
}

template<typename T>
Mat_<T>& Mat_<T>::operator =(const Mat_& m) {
  Release();
  rows = m.rows, cols = m.cols, step = m.step;
  data = (T*)m.data;
  return *this;
}

template<typename T>
Mat_<T>& Mat_<T>::operator -() {
  for (size_t i = 0; i < rows; ++i) {
    for (size_t j = 0; j < cols; ++j) {
      (*this)(i, j) = -(*this)(i, j);
    }
  }
}

template<typename T1, typename T2>
Mat_<T1>& operator +(const Mat_<T1>& m1, const Mat_<T2>& m2) {
  Mat_<T1> m;
  if (m1.rows != m2.rows || m1.cols != m2.cols)
    return m;
  m.rows = m1.rows; m.cols = m.step = m1.cols;
  if (!(data = new T1[m.rows * m.cols]))
    Err("Error in allocating memory in the overload of operator +.");
  for (size_t i = 0; i < m.rows; ++i) {
    for (size_t j = 0; j < m.cols; ++j) {
      m(i, j) = T1(m1(i, j) + m2(i, j));
    }
  }
}

template<typename T>
template<typename Tm>
Mat_<T>& Mat_<T>::operator +=(const Mat_<Tm>& m) {
  if (rows == m.rows && cols == m.cols) {
    for (size_t i = 0; i < rows; ++i) {
      for (size_t j = 0; j < cols; ++j) {
        (*this)(i, j) = T((*this)(i, j) + m(i, j));
      }
    }
  }
  return *this;
}

template<typename T>
Mat_<T>& operator -(const Mat_<T>& m1, const Mat_<T>& m2) {
  Mat_<T> m;
  if (m1.rows != m2.rows || m1.cols != m2.cols)
    return m;
  m.rows = m1.rows; m.cols = m.step = m1.cols;
  if (!(data = new T[m.rows * m.cols]))
    Err("Error in allocating memory in the overload of operator +.");
  for (size_t i = 0; i < m.rows; ++i) {
    for (size_t j = 0; j < m.cols; ++j) {
      m(i, j) = m1(i, j) - m2(i, j);
    }
  }
}

template<typename T>
template<typename Tm>
Mat_<T>& Mat_<T>::operator -=(const Mat_<Tm>& m) {
  if (rows == m.rows && cols == m.cols) {
    for (size_t i = 0; i < rows; ++i) {
      for (size_t j = 0; j < cols; ++j) {
        (*this)(i, j) = T((*this)(i, j) - m(i, j));
      }
    }
  }
  return *this;
}