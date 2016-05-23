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
  Mat_(size_t rows_, size_t cols_, size_t step_, T init_v);
  Mat_(const Mat_& m); // calls operator =
  template<size_t C>
  Mat_(T a[][C], size_t rows_);
  // destructor
  // will not deallocate memory. Call Release() to deallocate memory
  // if necessary.
  ~Mat_();

  // allocates memory
  void Allocate();
  void Allocate(T init_v);
  void Allocate(size_t rows_, size_t cols_);
  void Allocate(size_t rows_, size_t cols_, T init_v);
  void Allocate(const Mat_<T>& m);

  // releases old memory and re-allocates new one
  void Reallocate();
  void Reallocate(T init_v);
  void Reallocate(size_t rows_, size_t cols_);
  void Reallocate(size_t rows_, size_t cols_, T init_v);
  void Reallocate(const Mat_<T>& m);

  void Release(); // deallocates memory and reset all elements in the class

  bool Empty() const; // returns true if data == nullptr

  // returns reference to a specified element
  T& operator ()(int row, int col);
  T& operator ()(const Point& pt);
  // returns const reference to a specified element
  const T& operator ()(int row, int col) const;
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
Mat_<T>::Mat_(size_t rows_, size_t cols_, size_t step_, T init_v)
    : rows(rows_), cols(cols_), step(step_) {
  Allocate(init_v);
}

template<typename T>
Mat_<T>::Mat_(const Mat_& m) {
  *this = m;
}

template<typename T>
template<size_t C>
Mat_<T>::Mat_(T a[][C], size_t rows_) {
  rows = rows_; cols = C; step = cols;
  Allocate();
  for (size_t i = 0; i < rows; ++i) {
    for (size_t j = 0; j < cols; ++j) {
      (*this)(i, j) = a[i][j];
    }
  }
}

template<typename T>
Mat_<T>::~Mat_() {}

template<typename T>
void Mat_<T>::Allocate() {
  if (!(data = new (std::nothrow) T[rows * cols]))
    Err("Error in allocating memory for Mat_<T>::data.");
  if (step != cols)
    step = cols;
}

template<typename T>
void Mat_<T>::Allocate(T init_v) {
  if (!(data = new (std::nothrow) T[rows * cols]{init_v}))
    Err("Error in allocating memory for Mat_<T>::data.");
  if (step != cols)
    step = cols;
}

template<typename T>
void Mat_<T>::Allocate(size_t rows_, size_t cols_) {
  rows = rows_; cols = cols_;
  Allocate();
}

template<typename T>
void Mat_<T>::Allocate(size_t rows_, size_t cols_, T init_v) {
  rows = rows_; cols = cols_;
  Allocate(init_v);
}

template<typename T>
void Mat_<T>::Allocate(const Mat_<T>& m) {
  Allocate(m.rows, m.cols);
  for (size_t i = 0; i < rows; ++i) {
    for (size_t j = 0; j < cols; ++j) {
      (*this)(i, j) = m(i, j);
    }
  }
}

template<typename T>
void Mat_<T>::Reallocate() {
  delete[] data;
  Allocate();
}

template<typename T>
void Mat_<T>::Reallocate(T init_v) {
  delete[] data;
  Allocate(init_v);
}

template<typename T>
void Mat_<T>::Reallocate(size_t rows_, size_t cols_) {
  rows = rows_; cols = cols_;
  Reallocate();
}

template<typename T>
void Mat_<T>::Reallocate(size_t rows_, size_t cols_, T init_v) {
  rows = rows_; cols = cols_;
  Reallocate(init_v);
}

template<typename T>
void Mat_<T>::Reallocate(const Mat_<T>& m) {
  Reallocate(m.rows, m.cols);
  for (size_t i = 0; i < rows; ++i) {
    for (size_t j = 0; j < cols; ++j) {
      (*this)(i, j) = m(i, j);
    }
  }
}

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
T& Mat_<T>::operator ()(int row, int col) {
  return data[row * step + col];
}

template<typename T>
T& Mat_<T>::operator ()(const Point& pt) {
  return data[pt.y * step + pt.x];
}

template<typename T>
const T& Mat_<T>::operator ()(int row, int col) const {
  return ((const T*)data)[row * step + col];
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
  m.Allocate(m1.rows, m1.cols);
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
  m.Allocate(m1.rows, m1.cols);
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
Mat_<T> Resize(const Mat_<T>& src, float fx = 1.f, float fy = 1.f) {
  if (fx < 0 || fy < 0)
    return src;

  Mat_<T> dst(lround(src.rows / fy), lround(src.cols / fx));
  dst.Allocate();

  for (size_t i = 0; i < dst.rows; ++i) {
    for (size_t j = 0; j < dst.cols; ++j) {
      // dst(i, j) is supposed to be src(y, x)
      float x = j * fx;
      float y = i * fy;
      // the fractional part of x and y
      float x_fract = x - int(x);
      float y_fract = y - int(y);
      float xy_mul_fract = x_fract * y_fract;
      // bilinear interpolation
      dst(i, j) = T(round(
          (1.f - x_fract - y_fract + xy_mul_fract) * src(int(y), int(x)) +
          (y_fract - xy_mul_fract) * src(int(y) + 1, int(x)) +
          (x_fract - xy_mul_fract) * src(int(y), int(x) + 1) +
          xy_mul_fract * src(int(y) + 1, int(x) + 1)));
    }
  }
  return dst;
}

static const Point mark_array[] = {
  Point{0, -1},
  Point{-1, 0}, Point{0, 0}, Point{1, 0},
  Point{0, 1},
};

#define ADAPTIVE2BACKGROUND false

// marks a point on an image
template<typename T>
void MarkPoint(Mat_<T>& img, const Point& center) {
  for (const auto& e : mark_array) {
    Point pt{center + e};
    if (pt.x > int(img.cols) || pt.y > int(img.rows))
      break;
#if ADAPTIVE2BACKGROUND
    img(pt) = img(pt) < 180 ? 255 : 0;
#else
    img(pt) = 255;
#endif
  }
}