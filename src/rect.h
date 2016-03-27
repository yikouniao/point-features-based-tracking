#pragma once

#include "point.h"

template<typename T> class Rect_ {
 public:
  // constructors
  Rect_();
  Rect_(T x_, T y_, T width_, T height_);
  Rect_(const Rect_& r);
  Rect_(const Point_<T>& pt, T width_, T height_);
  Rect_(const Point_<T>& pt1, const Point_<T>& pt2);

  // destructor
  ~Rect_();

  // the top-left corner
  Point_<T> TL() const;
  // the bottom-right corner
  Point_<T> BR() const;

  // perimeter 2 * (width + height)
  T Perimeter() const;
  // area (width * height)
  T Area() const;
  // Check whether it's empty
  bool Empty() const;

  // Convert its data type
  template<typename T2> operator Rect_<T2>() const;

  // Check whether the rectangle contains the point/rectangle(including bounds)
  bool Contain(const Point_<T>& pt) const;
  bool Contain(const Rect_<T>& r) const;

  // assignment operator
  Rect_<T>& operator =(const Rect_& r);

  // Extend the rectangle range according an outer point
  Rect_<T>& operator +=(const Point_<T>& pt);

  // Compute the intersection
  Rect_<T>& operator &=(const Rect_<T>& r);
  // Compute the union
  Rect_<T>& operator |=(const Rect_<T>& r);

  // the top-left corner Coordinates, width and height
  T x, y, width, height;
};

typedef Rect_<int> Rect2i;
typedef Rect_<float> Rect2f;
typedef Rect_<double> Rect2d;
typedef Rect2i Rect;

template<typename T>
Rect_<T>::Rect_() : x(0), y(0), width(0), height(0) {}

template<typename T>
Rect_<T>::Rect_(T x_, T y_, T width_, T height_)
    : x(x_), y(y_), width(width_), height(height_) {}

template<typename T>
Rect_<T>::Rect_(const Rect_& r) {
  *this = r;
}

template<typename T>
Rect_<T>::Rect_(const Point_<T>& pt, T width_, T height_) {
  x = pt.x;
  y = pt.y;
  width = width_;
  height = height_;
}

template<typename T>
Rect_<T>::Rect_(const Point_<T>& pt1, const Point_<T>& pt2) {
  x = pt1.x;
  y = pt1.y;
  width = pt2.x <= pt1.x ? 0 : pt2.x - pt1.x + 1;
  height = pt2.y <= pt1.y ? 0 : pt2.y - pt1.y + 1;
}

template<typename T>
Rect_<T>::~Rect_() {}

template<typename T>
Point_<T> Rect_<T>::TL() const {
  return Point_<T>(x, y);
}

template<typename T>
Point_<T> Rect_<T>::BR() const {
  return Empty() ? TL() : Point_<T>(x + width - 1, y + height - 1);
}

template<typename T>
T Rect_<T>::Perimeter() const {
  return Empty() ? 0 : T(2 * (width + height));
}

template<typename T>
T Rect_<T>::Area() const {
  return Empty() ? 0 : T(width * height);
}

template<typename T>
bool Rect_<T>::Empty() const {
  return width <= 0 || height <= 0;
}

template<typename T>
template<typename T2>
Rect_<T>::operator Rect_<T2>() const {
  return Rect_<T2>(T2(x), T2(y), T2(width), T2(height));
}

template<typename T>
bool Rect_<T>::Contain(const Point_<T>& pt) const {
  return pt.x >= x && pt.x < x + width && pt.y >= y && pt.y < y + height;
}

template<typename T>
bool Rect_<T>::Contain(const Rect_<T>& r) const {
  if (r.Empty())
    return true;
  return r.x >= x && r.x + r.width <= x + width &&
         r.y >= y && r.y + height <= y + height;
}

template<typename T>
bool operator ==(const Rect_<T>& r1, const Rect_<T>& r2) {
  return r1.x == r2.x && r1.y == r2.y &&
         r1.width == r2.width && r1.height == r2.height;
}

template<typename T>
bool operator !=(const Rect_<T>& r1, const Rect_<T>& r2) {
  return r1.x != r2.x || r1.y != r2.y ||
         r1.width != r2.width || r1.height != r2.height;
}

template<typename T>
Rect_<T>& Rect_<T>::operator =(const Rect_& r) {
  x = T(r.x);
  y = T(r.y);
  width = T(r.width);
  height = T(r.height);
  return *this;
}

template<typename T>
Rect_<T>& Rect_<T>::operator +=(const Point_<T>& pt) {
  if (!(this->Contain())) {
    if (pt.x < x) {
      width += x - pt.x;
      x = pt.x;
    } else if (pt.x >= x + width) {
      width += pt.x - x - 1;
    }
    if (pt.y < y) {
      height += y - pt.y;
      y = pt.y;
    } else if (pt.y >= y + height) {
      height += pt.y - y - 1;
    }
  }
  return *this;
}

// Compute the intersection
template<typename T>
Rect_<T>& operator &(const Rect_<T>& r1, const Rect_<T>& r2) {
  Rect_<T> intersection();
  if (r1.Empty() || r2.Empty()) {
    return intersection;
  }

  if (r1.x + r1.width - 1 < r2.x || r2.x + r2.width - 1 < r1.x ||
      r1.y + r1.height - 1 < r2.y || r2.y + r2.height - 1 < r1.y) {
    return intersection;
  }

  if (r1.x + r1.width > r2.x + r2.width) {
    if (r1.x > r2.x) {
      intersection.x = r1.x;
      intersection.width = r2.x + r2.width - r1.x;
    } else {
      intersection.x = r2.x;
      intersection.width = r2.width;
    }
  } else if (r1.x > r2.x) {
    intersection.x = r1.x;
    intersection.width = r1.width;
  } else {
    intersection.x = r2.x;
    intersection.width = r1.x + r1.width - r2.x;
  }

  if (r1.y + r1.height > r2.y + r2.height) {
    if (r1.y > r2.y) {
      intersection.y = r1.y;
      intersection.height = r2.y + r2.height - r1.y;
    } else {
      intersection.y = r2.y;
      intersection.height = r2.height;
    }
  } else if (r1.y > r2.y) {
    intersection.y = r1.y;
    intersection.height = r1.height;
  } else {
    intersection.y = r2.y;
    intersection.height = r1.y + r1.height - r2.y;
  }
}

template<typename T>
Rect_<T>& Rect_<T>::operator &=(const Rect_<T>& r) {
  *this = *this & r;
  return *this;
}

// Compute the union
template<typename T>
Rect_<T>& operator |(const Rect_<T>& r1, const Rect_<T>& r2) {
  Rect_<T> rect_union();
  if (r1.Empty()) {
    return r2;
  } else if (r2.Empty()) {
    return r1;
  }

  if (r1.x > r2.x) {
    rect_union.x = r2.x;
    if (r1.x + r1.width < r2.x + r2.width) {
      rect_union.width = r2.width;
    } else {
      rect_union.width = r1.x + r1.width - r2.x;
    }
  } else {
    rect_union.x = r1.x;
    if (r1.x + r1.width > r2.x + r2.width) {
      rect_union.width = r1.width;
    } else {
      rect_union.width = r2.x + r2.width - r1.x;
    }
  }

  if (r1.y > r2.y) {
    rect_union.y = r2.y;
    if (r1.y + r1.height < r2.y + r2.height) {
      rect_union.height = r2.height;
    } else {
      rect_union.height = r1.y + r1.height - r2.y;
    }
  } else {
    rect_union.y = r1.y;
    if (r1.y + r1.height > r2.y + r2.height) {
      rect_union.height = r1.height;
    } else {
      rect_union.height = r2.y + r2.height - r1.y;
    }
  }
}

template<typename T>
Rect_<T>& Rect_<T>::operator |=(const Rect_<T>& r) {
  *this = *this | r;
  return *this;
}