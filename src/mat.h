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
  Mat_(const Mat_& m); // calls operator =
  Mat_(size_t rows_, size_t cols_, T init_v);
  template<size_t C>
  Mat_(T a[][C], size_t rows_);
  // destructor
  // will not deallocate memory. Call Release() to deallocate memory
  // if necessary.
  ~Mat_();

  void Release(); // deallocates memory and reset all elements in the class

  bool Empty() const; // returns true if data == nullptr

  // releases old memory, allocate new memory and assign the new contents
  void Clone(const Mat_& m);
  void Creat(size_t rows_, size_t cols_, T init_v);

  // returns reference to a specified element
  T& operator ()(int i, int j);
  T& operator ()(const Point& pt);
  // returns const reference to a specified element
  const T& operator ()(int i, int j) const;
  const T& operator ()(const Point& pt) const;

  Mat_<T> operator ()(const Rect_<T>& roi) const;

  // assignment operator, shallow copy, will not release this->data
  Mat_<T>& operator =(const Mat_& m);

  Mat_<T>& operator -();
  template<typename Tm>
  Mat_<T>& operator +=(const Mat_<Tm>& m);
  template<typename Tm>
  Mat_<T>& operator -=(const Mat_<Tm>& m);

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
  if (!(data = new (std::nothrow) T[rows * cols]{init_v}))
    Err("Error in allocating memory for Mat_<T>::data in the constructor.");
}

template<typename T>
template<size_t C>
Mat_<T>::Mat_(T a[][C], size_t rows_) {
  rows = rows_; cols = C; step = cols;
  if (!(data = new (std::nothrow) T[rows * cols]))
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
  if (!(data = new (std::nothrow) T[rows * cols]))
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
  if (!(data = new (std::nothrow) T[rows * cols]{init_v}))
    Err("Error in allocating memory for Mat_<T>::data in the constructor.");
}

template<typename T>
T& Mat_<T>::operator ()(int i, int j) {
  return data[i * step + j];
}

template<typename T>
T& Mat_<T>::operator ()(const Point& pt) {
  return data[pt.y * step + pt.x];
}

template<typename T>
const T& Mat_<T>::operator ()(int i, int j) const {
  return ((const T*)data)[i * step + j];
}

template<typename T>
const T& Mat_<T>::operator ()(const Point& pt) const {
  return ((const T*)data)[pt.y * step + pt.x];
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
  if (!(data = new (std::nothrow) T1[m.rows * m.cols]))
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
  if (!(data = new (std::nothrow) T[m.rows * m.cols]))
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

// resizes a matrix (allocate memory for new matrix) by bilinear interpolation.
// fx and fy are scale factors. The size of the new matrix is:
// rows. = lround(src.rows / fy); cols. = lround(src.cols / fx);
// If fx or fy is less than 0, it's considered a wrong call and no memory
// will be allocated. It returns src in this case.
template<typename T>
Mat_<T> Resize(const Mat_<T>& src, float fx = 1, float fy = 1) {
  if (fx < 0 || fy < 0)
    return src;
  Mat_<T> dst(lround(src.rows / fy), lround(src.cols / fx));
  if (!(dst.data = new (std::nothrow) T[dst.rows * dst.cols]))
    Err("Error in allocating memory while resizing a matrix.");

  for (size_t i = 0; i < dst.rows; ++i) {
    for (size_t j = 0; j < dst.cols; ++j) {
      // dst(i, j) is supposed to be src(y, x)
      float x = float(j) * src.cols / dst.cols;
      float y = float(i) * src.rows / dst.rows;
      // the fractional part of x and y
      float x_fract = x - int(x);
      float y_fract = y - int(y);
      // bilinear interpolation
      dst(i, j) = T((1 - x_fract) * (1 - y_fract) * src(int(y), int(x)) +
                    (1 - x_fract) * y_fract * src(int(y) + 1, int(x)) +
                    x_fract * (1 - y_fract) * src(int(y), int(x) + 1) +
                    x_fract * y_fract * src(int(y) + 1, int(x) + 1));
    }
  }
  return dst;
}