#pragma once

#include "types.h"
#include <vector>

void ORBTemp();
static void GetKeyPoints(int nfeatures, int border_width, const std::vector<Rect>& layer_info, float scale_factor);
static void HarrisResponses(
    std::vector<KeyPoint>& keypoints, const Mat& img,
    const std::vector<Rect>& layer_info, size_t block_size, float k);
static void GetPyramid(const Mat& img, std::vector<Mat>& pyramid,
                       size_t n_levels, float fx, float fy);

// class ORBDescriptor {