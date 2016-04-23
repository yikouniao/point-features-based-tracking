#include "main.h"
#include "orb.h"
#include "file.h"
#include "err.h"
#include "fstream"

using namespace std;

void GetRelPos(std::vector<Pointf>& obj_rel) {
  obj_rel.clear();
  obj_rel.resize(img_num);

  ifstream fs(img_file_path + rel_pos_fname);
  if (!fs)
    Err("Cannot open " + img_file_path + rel_pos_fname);

  for (auto& e : obj_rel) {
    fs >> e.x >> e.y;
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
  vector<Pointf> obj_rel; // the real positions of the object
  GetRelPos(obj_rel);

  int img_cnt = 0;
  string img_ref_fname = img_file_path + img_fname; // reference image
  string img_rt_fname = img_ref_fname;              // real-time image

  OrbMethod* orb = new OrbMethod();

  // first orb
  Mat img_ref = ImgRead(img_ref_fname);
  vector<KeyPoint> keypoints_ref;
  OrbDescriptors descriptors_ref;
  orb->OrbImpl(img_ref, keypoints_ref, descriptors_ref);

  // other imgs
  for (++img_cnt; img_cnt < 255; ++img_cnt) {
    GetNextImgFileName(img_rt_fname);
    // orb
  }

  img_ref.Release();

  delete orb;

  return 0;
}