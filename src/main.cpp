#include "main.h"
#include <fstream>
#include "orb.h"
#include "file.h"
#include "err.h"
#include "affine-transf.h"

using namespace std;

void GetRelPos(std::vector<Pointf>& obj_rel) {
  ifstream fs(img_file_path + rel_pos_fname);
  if (!fs)
    Err("Cannot open " + img_file_path + rel_pos_fname);

  for (auto& e : obj_rel) {
    fs >> e.x >> e.y;
  }
}

void SaveResPos(std::vector<Pointf>& obj_res) {
  ofstream fs(results_path + res_pos_fname);
  if (!fs)
    Err("Cannot open " + results_path + res_pos_fname);

  for (auto& e : obj_res) {
    fs << e.x << '\t' << e.y << '\n';
  }
}

void GetNextImgFileName(std::string& img_name) {
  size_t num_pos = img_name.size() - 5;
  if (img_name[num_pos] == '9') {
    img_name[num_pos] = '0';
    --num_pos;
    if (img_name[num_pos] == '9') {
      img_name[num_pos] = '0';
      --num_pos;
      if (img_name[num_pos] == '9') {
        img_name[num_pos] = '0';
        --num_pos;
        if (img_name[num_pos] == '9') {
          img_name[num_pos] = '0';
        } else {
          ++img_name[num_pos];
        }
      } else {
        ++img_name[num_pos];
      }
    } else {
      ++img_name[num_pos];
    }
  } else {
    ++img_name[num_pos];
  }
}

int main(int argc, char** argv) {
  // the real and result positions of the object
  vector<Pointf> obj_rel(img_num), obj_res(img_num);
  GetRelPos(obj_rel);
  obj_res[0] = obj_rel[0];

  int img_cnt = 0;
  int ref_cnt = 0;
  string img_ref_fname = img_file_path + img_fname; // reference image
  string img_rt_fname = img_ref_fname;              // real-time image
  string result_fname = results_path + img_fname;   // result image

  OrbMethod* orb = new OrbMethod();

  // orb for the first image
  Mat img_ref = ImgRead(img_ref_fname);
  vector<KeyPoint> keypoints_ref;
  OrbDescriptors descriptors_ref;
  orb->OrbImpl(img_ref, keypoints_ref, descriptors_ref);

  // Save the result
  MarkPoint(img_ref, obj_res[0]);
  ImgWrite(result_fname, img_ref);
  img_ref.Release();

  // Save infomation of last 3 images as spare reference
  vector<vector<KeyPoint>> keypoints_spare(spare_num);
  vector<OrbDescriptors> descriptors_spare(spare_num);

  // Process real-time images
  for (++img_cnt; img_cnt < img_num; ++img_cnt) {
    GetNextImgFileName(img_rt_fname);

    // orb
    Mat img_rt = ImgRead(img_rt_fname);
    vector<KeyPoint> keypoints_rt;
    OrbDescriptors descriptors_rt;
    orb->OrbImpl(img_rt, keypoints_rt, descriptors_rt);

    // matches
    vector<DescMatch> matches;
    OrbMatch(descriptors_ref, descriptors_rt, matches);

    // If there're no enouge matches, reselect reference image from spare ones,
    // which has the biggest matches.size().
    if (matches.size() < 18) {
      vector<vector<DescMatch>> matches_spare(spare_num);
      size_t best_matches_size = 0;
      int best_matches_idx = 0;
      for (int i = 0; i < spare_num; ++i) {
        OrbMatch(descriptors_ref, descriptors_spare[i], matches_spare[i]);
        if (best_matches_size < matches_spare[i].size()) {
          best_matches_size = matches_spare[i].size();
          best_matches_idx = i;
        }
      }
      keypoints_ref = keypoints_spare[best_matches_idx];
      descriptors_ref = descriptors_spare[best_matches_idx];
      OrbMatch(descriptors_ref, descriptors_rt, matches);
      ref_cnt = max(img_cnt - best_matches_idx - 1, 0);
    }

    // affine transformation
    AffineTransf{GetAffineTransf(keypoints_ref, keypoints_rt, matches)}.
        GetDstPoint(obj_res[ref_cnt], obj_res[img_cnt]);

    // Save results
    GetNextImgFileName(result_fname);
    MarkPoint(img_rt, obj_res[img_cnt]);
    MarkPoint(img_rt, obj_rel[img_cnt]);
    ImgWrite(result_fname, img_rt);
    img_rt.Release();

    // Save as spare reference
    keypoints_spare.erase(--keypoints_spare.end());
    keypoints_spare.insert(keypoints_spare.begin(), keypoints_rt);
    descriptors_spare.erase(--descriptors_spare.end());
    descriptors_spare.insert(descriptors_spare.begin(), descriptors_rt);
  }

  delete orb;

  SaveResPos(obj_res);

  return 0;
}