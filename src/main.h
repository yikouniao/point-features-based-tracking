#pragma once

#include <vector>
#include "types.h"

// gets real positions from file in rel_pos_fname
void GetRelPos(std::vector<Pointf>& obj_rel);

// saves result positions to file in res_pos_fname
void SaveResPos(std::vector<Pointf>& obj_res);

// gets next file name.
// For example, input "ngsimu0000.bmp" and you can get "ngsimu0001.bmp".
void GetNextImgFileName(std::string& img_name);