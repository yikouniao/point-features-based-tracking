#include "orb.h"
#include <iostream>
#include "err.h"
#include "file.h"
#include "types.h"
#include "blur.h"

using namespace std;

string f_name0 = "complex building/simu0014.bmp";
string f_name1 = "complex building/simu0076.bmp";

int main(int argc, char** argv) {
  Mat img0 = ImgRead(f_name0);

  OrbMethod* orbd = new OrbMethod();
  vector<KeyPoint> keypoints0;
  OrbDescriptors descriptors0;
  orbd->OrbImpl(img0, keypoints0, descriptors0);

  //MarkKeyPoints(img, keypoints);
  //ImgWrite("../result.bmp", img);

  img0.Release();

  Mat img1 = ImgRead(f_name1);

  vector<KeyPoint> keypoints1;
  OrbDescriptors descriptors1;
  orbd->OrbImpl(img1, keypoints1, descriptors1);

  //MarkKeyPoints(img, keypoints);
  //ImgWrite("../result.bmp", img);

  img1.Release();

  delete orbd;

  std::vector<DescMatch> matches;
  OrbMatch(descriptors0, descriptors1, matches);

  return 0;
}