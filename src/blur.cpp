#include "blur.h"

using namespace std;

void GaussianBlur(const std::vector<Mat>& src, std::vector<Mat>& dst,
                  int kwidth, int kheight, double sigmax, double sigmay) {
  if (src.size() == dst.size()) {
    for (size_t i = 0; i < src.size(); ++i)
      GaussianBlur(src[i], dst[i], kwidth, kheight, sigmax, sigmay);
  }
}

void GaussianBlur(const Mat& src, Mat& dst, int kwidth, int kheight,
                  double sigmax, double sigmay) {
  if (!(kwidth % 2) || !(kheight % 2) || kwidth < 0 || kheight < 0 ||
      (kwidth == 1 && kheight == 1) || sigmax <= 0 || sigmay <= 0) {
    dst = src;
    return;
  }

  Mat dst_(src.rows, src.cols);
  dst_.Allocate();

  // Get kernels
  vector<float> kernelx, kernely;
  GetGaussianKernel(kernelx, kwidth, sigmax);
  GetGaussianKernel(kernely, kheight, sigmay);

  // Blur
  GaussianBlurImpl(src, dst_, kernelx, kernely);

  dst.Release();
  dst = dst_;
}

static void GetGaussianKernel(
    std::vector<float>& kernel, int ksize, double sigma) {
  kernel.clear();
  kernel.resize(ksize);
  double kernelsum{0};
  double gama = -.5 / (sigma * sigma);

  int half_ksize_l = ksize / 2;
  int half_ksize_g = half_ksize_l + 1;

  // Compute the first half kernel and the central point
  for (int i = 0; i < half_ksize_g; ++i) {
    double distance = i - (ksize - 1) * .5;
    kernel[i] = float(exp(distance * distance * gama));
    kernelsum += kernel[i];
  }
  // The last half kernel equals to the first half symmetrically
  for (int i = ksize - 1; i > half_ksize_l; --i) {
    kernel[i] = kernel[ksize - 1 - i];
    kernelsum += kernel[i];
  }

  kernelsum = 1. / kernelsum;

  for (int i = 0; i < ksize; ++i) {
    kernel[i] = float(kernel[i] * kernelsum);
  }
}

#define NOT_EDGE_X 0
#define LEFT_EDGE_X 1
#define RIGHT_EDGE_X 2
#define NOT_EDGE_Y 3
#define TOP_EDGE_Y 4
#define BOTTOM_EDGE_Y 5

static void GaussianBlurImpl(const Mat& src, Mat& dst,
    const std::vector<float>& kernelx, const std::vector<float>& kernely) {
  Matf dst_(src.rows, src.cols); // interval result for X-axis blurring
  dst_.Allocate();

  // [drange_l, drange_g) is the range of data inside the image edge in X-axis
  size_t drange_l = kernelx.size() / 2;
  size_t drange_g = dst_.cols - drange_l;
  std::vector<int> offsets(kernelx.size());
  GetAddrOffsets(src, Point(), offsets, NOT_EDGE_X);
  for (size_t i = 0; i < dst_.rows; ++i) {
    for (size_t j = drange_l; j < drange_g; ++j) {
      dst_(i, j) = WeightedAverage(&src(i, j), offsets, kernelx);
    }
  }
  // the left and right edge
  for (size_t j = 0, t = kernelx.size() - 1; j < drange_l; ++j) {
    GetAddrOffsets(src, Point(j, 0), offsets, LEFT_EDGE_X);
    for (size_t i = 0; i < dst_.rows; ++i) {
    dst_(i, j) = WeightedAverage(&src(i, j), offsets, kernelx);
    }
  }
  for (size_t j = drange_g, t = j - 2 + j; j < dst_.cols; ++j) {
    GetAddrOffsets(src, Point(j, 0), offsets, RIGHT_EDGE_X);
    for (size_t i = 0; i < dst_.rows; ++i) {
    dst_(i, j) = WeightedAverage(&src(i, j), offsets, kernelx);
    }
  }

  // [drange_l, drange_g) is the range of data inside the image edge in Y-axis
  drange_l = kernely.size() / 2;
  drange_g = dst.rows - drange_l;
  offsets.resize(kernely.size());
  GetAddrOffsets(dst, Point(), offsets, NOT_EDGE_Y);
  for (size_t i = drange_l; i < drange_g; ++i) {
    for (size_t j = 0; j < dst.cols; ++j) {
      dst(i, j) = uchar(round(WeightedAverage(&dst_(i, j), offsets, kernely)));
    }
  }
  // the top and bottom edge
  for (size_t i = 0, t = kernely.size() - 1; i < drange_l; ++i) {
    GetAddrOffsets(dst, Point(0, i), offsets, TOP_EDGE_Y);
    for (size_t j = 0; j < dst.cols; ++j) {
      dst(i, j) = uchar(round(WeightedAverage(&dst_(i, j), offsets, kernely)));
    }
  }
  for (size_t i = drange_g, t = i - 2 + i; i < dst.rows; ++i) {
    GetAddrOffsets(dst, Point(0, i), offsets, BOTTOM_EDGE_Y);
    for (size_t j = 0; j < dst.cols; ++j) {
      dst(i, j) = uchar(round(WeightedAverage(&dst_(i, j), offsets, kernely)));
    }
  }
  dst_.Release();
}

static void GetAddrOffsets(const Mat& img, const Point& pt,
                           std::vector<int>& offsets, int edge_type) {
  // [0, edge_l) || [edge_g, cols/rows) is the edge
  int edge_l = int(offsets.size() / 2);
  int edge_g = int(img.cols) - edge_l;

  if (edge_type == NOT_EDGE_X) { // not edge
    offsets[0] = -edge_l;
    for (size_t i = 1; i < offsets.size(); ++i) {
      offsets[i] = offsets[i - 1] + 1;
    }
  } else if (edge_type == LEFT_EDGE_X) { // left edge
    int i = offsets.size() - 1;
    offsets[i] = edge_l; // half of offsets.size()
    int symmetry_axis = edge_l - pt.x; // position of symmetry axis
    for (--i; i >= symmetry_axis; --i) {
      offsets[i] = offsets[i + 1] - 1;
    }
    for (; i >= 0; --i) {
      offsets[i] = offsets[symmetry_axis + symmetry_axis - i];
    }
  } else if (edge_type == RIGHT_EDGE_X) { // right edge
    // symmetrical with left edge
    int i = 0;
    offsets[i] = -edge_l;
    int symmetry_axis = int(img.cols) - pt.x + edge_l - 1;
    for (++i; i <= symmetry_axis; ++i) {
      offsets[i] = offsets[i - 1] + 1;
    }
    for (; i < int(offsets.size()); ++i) {
      offsets[i] = offsets[symmetry_axis + symmetry_axis - i];
    }
  } else if (edge_type == NOT_EDGE_Y) { // not edge
    offsets[0] = -edge_l * int(img.step);
    for (size_t i = 1; i < offsets.size(); ++i) {
      offsets[i] = offsets[i - 1] + int(img.step);
    }
  } else if (edge_type == TOP_EDGE_Y) { // top edge
    int i = offsets.size() - 1;
    offsets[i] = edge_l * int(img.step); // half of offsets.size()
    int symmetry_axis = edge_l - pt.y; // position of symmetry axis
    for (--i; i >= symmetry_axis; --i) {
      offsets[i] = offsets[i + 1] - int(img.step);
    }
    for (; i >= 0; --i) {
      offsets[i] = offsets[symmetry_axis + symmetry_axis - i];
    }
  } else if (edge_type == BOTTOM_EDGE_Y) { // bottom edge
    // symmetrical with top edge
    int i = 0;
    offsets[i] = -edge_l * int(img.step);
    int symmetry_axis = int(img.rows) - pt.y + edge_l - 1;
    for (++i; i <= symmetry_axis; ++i) {
      offsets[i] = offsets[i - 1] + int(img.step);
    }
    for (; i < int(offsets.size()); ++i) {
      offsets[i] = offsets[symmetry_axis + symmetry_axis - i];
    }
  } else {
    fill(offsets.begin(), offsets.end(), 0);
  }
}