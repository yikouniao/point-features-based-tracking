#pragma once

#include "point.h"
#include "keypoint.h"
#include "rect.h"
#include "mat.h"
#include "descmatch.h"

using schar = signed char;
using uchar = unsigned char;
using ushort = unsigned short;
using uint = unsigned int;
using int64 = __int64;
using uint64 = unsigned __int64;

#ifndef PI
#define PI 3.1415926535897932384626433832795
#endif
#ifndef RAD2DEG
#define RAD2DEG(x) (x * 180. / PI)
#endif
#ifndef DEG2RAD
#define DEG2RAD(x) (x * PI / 180.)
#endif