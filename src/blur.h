#pragma once

#include "types.h"

// blurs an image with Gaussian kernel
// kwidth and kheight are size of the kernel
void GaussianBlur(const Mat& src, Mat& dst, int kwidth, int kheight);