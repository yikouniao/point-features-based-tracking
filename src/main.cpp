#include "main.h"
#include "orb.h"
#include "file.h"
#include "err.h"
#include "affine-transf.h"
#include "fstream"

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
  ofstream fs(img_file_path + res_pos_fname);
  if (!fs)
    Err("Cannot open " + img_file_path + res_pos_fname);

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
  string img_ref_fname = img_file_path + img_fname; // reference image
  string img_rt_fname = img_ref_fname;              // real-time image

  OrbMethod* orb = new OrbMethod();

  // orb for the first image
  Mat img_ref = ImgRead(img_ref_fname);
  vector<KeyPoint> keypoints_ref;
  OrbDescriptors descriptors_ref;
  orb->OrbImpl(img_ref, keypoints_ref, descriptors_ref);

  // other imgs
  for (++img_cnt; img_cnt < 255; ++img_cnt) {
    GetNextImgFileName(img_rt_fname);
    // orb
    Mat img_rt = ImgRead(img_rt_fname);
    vector<KeyPoint> keypoints_rt;
    OrbDescriptors descriptors_rt;
    orb->OrbImpl(img_rt, keypoints_rt, descriptors_rt);
    img_rt.Release();
    // match
    vector<DescMatch> matches;
    OrbMatch(descriptors_ref, descriptors_rt, matches);
    // affine transformation
    AffineTransf{GetAffineTransf(keypoints_ref, keypoints_rt, matches)}.
        GetDstPoint(obj_rel[img_cnt], obj_res[img_cnt]);
  }

  img_ref.Release();

  delete orb;

  SaveResPos(obj_res);

  return 0;
}