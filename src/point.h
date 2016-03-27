#pragma once

template<typename T> class Point_ {
 public:
  // constructors
  Point_();
  Point_(T x_, T y_);
  Point_(const Point_& pt);

  // destructors
  ~Point_();

  // conversion to another data type
  template<typename T2> operator Point_<T2>() const;

  // dot product
  T Dot(const Point_& pt) const;
  // dot product, return a double-precision result
  double DDot(const Point_& pt) const;
  // L2 norm
  double Norm() const;
  //// Check whether the point is inside a specified rectangle
  //bool inside(const Rect_<T>& r) const;

  // operator overloadings
  Point_<T>& operator =(const Point_& pt);
  Point_<T>& operator +=(const Point_& pt);
  Point_<T>& operator -=(const Point_& pt);
  template<typename Ta>
  Point_<T>& operator *=(const Ta a);
  template<typename Ta>
  Point_<T>& operator /=(const Ta a);

  T x, y; // the point coordinates
};

typedef Point_<int> Point2i;
typedef Point_<float> Point2f;
typedef Point_<double> Point2d;
typedef Point2i Point;

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

// operator overloadings
template<typename T>
Point_<T> operator +(const Point_<T>& pta, const Point_<T>& ptb) {
  return Point_(pta.x + ptb.x, pta.y + ptb.y);
}

template<typename T>
Point_<T> operator -(const Point_<T>& pta, const Point_<T>& ptb) {
  return Point_(pta.x - ptb.x, pta.y - ptb.y);
}

template<typename T, typename Ta>
Point_<T> operator *(const Point_<T>& pt, const Ta a) {
  return Point_(T(pt.x * a), T(pt.y * a));
}

template<typename T, typename Ta>
Point_<T> operator *(const Ta a, const Point_<T>& pt) {
  return Point_(T(pt.x * a), T(pt.y * a));
}

template<typename T, typename Ta>
Point_<T> operator /(const Point_<T>& pt, const Ta a) {
  return Point_(T(pt.x / a), T(pt.y / a));
}

template<typename T>
Point_<T> operator -(const Point_<T>& pt) {
  return Point_(-pt.x, -pt.y);
}

template<typename T>
bool operator ==(const Point_<T>& pta, const Point_<T>& ptb) {
  return pta.x == ptb.x && pta.y == ptb.y;
}

template<typename T>
bool operator !=(const Point_<T>& pta, const Point_<T>& ptb) {
  return pta.x != ptb.x || pta.y != ptb.y;
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