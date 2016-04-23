#pragma once

#include <vector>
#include "types.h"

static const std::string img_file_path = "bridge/";
static const std::string img_fname = "ngsimu0000.bmp";
static const std::string rel_pos_fname = "relPosition.txt";
static const int img_num = 225;

// gets real positions from file in rel_pos_fname
void GetRelPos(std::vector<Pointf>& obj_rel);

// gets next file name.
// For example, input "ngsimu0000.bmp" and you can get "ngsimu0001.bmp".
void GetNextImgFileName(std::string& img_name);