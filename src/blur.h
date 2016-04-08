#pragma once

#include "types.h"
#include <vector>

// blurs an image pyramid with Gaussian kernel
void GaussianBlur(const std::vector<Mat>& src, std::vector<Mat>& dst,
                  int kwidth, int kheight, double sigmax, double sigmay);

// blurs an image with Gaussian kernel
// kwidth and kheight are size of the kernel
// sigmax and sigmay are Gaussian kernel standard deviations: sigma
// calls void GetGaussianKernel(...) to get kernel coefficients in x and y
// directions respectively.
// Blur a image in two directions respectively can be much faster
// than in two-dimensional matrix directly. The complexity can be reduced
// from O(kwidth*kheight*src.rows*src.cols)
// to O(kwidth*src.rows*src.cols) + O(kheight*src.rows*src.cols).
void GaussianBlur(const Mat& src, Mat& dst, int kwidth, int kheight,
                  double sigmax, double sigmay);

// gets Gaussian kernel coefficients in one direction.
static void GetGaussianKernel(
    std::vector<float>& kernel, int ksize, double sigma);

// implements Gaussian kernel blurring, callled by void GaussianBlur(...)
static void GaussianBlurImpl(const Mat& src, Mat& dst,
    const std::vector<float>& kernelx, const std::vector<float>& kernely);

#define NOT_EDGE_X 0
#define LEFT_EDGE_X 1
#define RIGHT_EDGE_X 2
#define NOT_EDGE_Y 3
#define TOP_EDGE_Y 4
#define BOTTOM_EDGE_Y 5

// gets data address offsets
static void GetAddrOffsets(const Mat& img, const Point& central_pt,
                           std::vector<int>& offsets, int edge_type);

// computes the weighted average with coef
// data points to the central data of an array, the length of the computed data
// equals to that of coef.
// data + offsets[i] * sizeof(uchar) == &data[i+1]
static uchar WeightedAverage(const uchar* data, std::vector<int>& offsets,
                             const std::vector<float>& coef);