#pragma once

#include "types.h"
#include <vector>

void ORB();
static void GetKeyPoints();
static void HarrisResponses(
    std::vector<KeyPoint>& keypoints, const Mat& img,
    const std::vector<Rect>& layer_info, size_t block_size, float k);