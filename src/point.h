#pragma once

template<typename T> class Point_ {
 public:
  // constructors
  Point_();
  Point_(T x_, T y_);
  Point_(const Point_& pt);

  // destructor
  ~Point_();

  // conversion to another data type
  template<typename T2> operator Point_<T2>() const;

  // dot product
  T Dot(const Point_& pt) const;
  // dot product, return a double-precision result
  double DDot(const Point_& pt) const;
  // L2 norm
  double Norm() const;

  // operators overloading
  Point_<T>& operator =(const Point_& pt);
  Point_<T>& operator +=(const Point_& pt);
  Point_<T>& operator -=(const Point_& pt);
  template<typename Ta>
  Point_<T>& operator *=(const Ta a);
  template<typename Ta>
  Point_<T>& operator /=(const Ta a);

  T x, y; // the point coordinates
};

using Pointi = Point_<int>;
using Pointf = Point_<float>;
using Pointd = Point_<double>;
using Point = Pointi;

template<typename T>
Point_<T>::Point_() : x(0), y(0) {}

template<typename T>
Point_<T>::Point_(T x_, T y_) : x(x_), y(y_) {}

template<typename T>
Point_<T>::Point_(const Point_& pt) {
  *this = pt;
}

template<typename T>
Point_<T>::~Point_() {}

template<typename T>
template<typename T2>
Point_<T>::operator Point_<T2>() const {
  return Point_<T2>(T2(x), T2(y));
}

template<typename T>
T Point_<T>::Dot(const Point_& pt) const {
  return T(x * pt.x + y * pt.y);
}

template<typename T>
double Point_<T>::DDot(const Point_& pt) const {
  return x * double(pt.x) + y * double(pt.y);
}

template<typename T>
double Point_<T>::Norm() const {
  return sqrt(double(x * x + y * y));
}

// operators overloading
template<typename T>
Point_<T> operator +(const Point_<T>& pt1, const Point_<T>& pt2) {
  return Point_<T>(pt1.x + pt2.x, pt1.y + pt2.y);
}

template<typename T>
Point_<T> operator -(const Point_<T>& pt1, const Point_<T>& pt2) {
  return Point_<T>(pt1.x - pt2.x, pt1.y - pt2.y);
}

template<typename T, typename Ta>
Point_<T> operator *(const Point_<T>& pt, const Ta a) {
  return Point_<T>(T(pt.x * a), T(pt.y * a));
}

template<typename T, typename Ta>
Point_<T> operator *(const Ta a, const Point_<T>& pt) {
  return Point_<T>(T(pt.x * a), T(pt.y * a));
}

template<typename T, typename Ta>
Point_<T> operator /(const Point_<T>& pt, const Ta a) {
  return Point_<T>(T(pt.x / a), T(pt.y / a));
}

template<typename T>
Point_<T> operator -(const Point_<T>& pt) {
  return Point_<T>(-pt.x, -pt.y);
}

template<typename T>
bool operator ==(const Point_<T>& pt1, const Point_<T>& pt2) {
  return pt1.x == pt2.x && pt1.y == pt2.y;
}

template<typename T>
bool operator !=(const Point_<T>& pt1, const Point_<T>& pt2) {
  return pt1.x != pt2.x || pt1.y != pt2.y;
}

template<typename T>
Point_<T>& Point_<T>::operator =(const Point_& pt) {
  x = T(pt.x);
  y = T(pt.y);
  return *this;
}

template<typename T>
Point_<T>& Point_<T>::operator +=(const Point_& pt) {
  x = T(x + pt.x);
  y = T(y + pt.y);
  return *this;
}

template<typename T>
Point_<T>& Point_<T>::operator -=(const Point_& pt) {
  x = T(x - pt.x);
  y = T(y - pt.y);
  return *this;
}

template<typename T>
template<typename Ta>
Point_<T>& Point_<T>::operator *=(const Ta a) {
  x = T(x * a);
  y = T(y * a);
  return *this;
}

template<typename T>
template<typename Ta>
Point_<T>& Point_<T>::operator /=(const Ta a) {
  x = T(x / a);
  y = T(y / a);
  return *this;
}