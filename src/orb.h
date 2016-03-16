#pragma once
#include "rect.h"
#include <vector>
#include "keypoint.h"

void ORB();
static void HarrisResponses(
    std::vector<KeyPoint>& keypoints, const Mat& img,
    const std::vector<Rect>& layer_info, size_t block_size, float k);