#include "orb.h"
#include <iostream>
#include "test-cv.h"
#include "err.h"
#include "file.h"
#include "types.h"
#include "blur.h"

using namespace std;

#define TEST 1

string f_name0 = "complex building/simu0014.bmp";
string f_name1 = "complex building/simu0076.bmp";

int main(int argc, char** argv) {
#if TEST
  test(f_name0, f_name1);
#else
  Mat img = ImgRead(f_name0);

  OrbMethod* orbd = new OrbMethod();
  vector<KeyPoint> keypoints;
  OrbDescriptors descriptors;
  orbd->OrbImpl(img, keypoints, descriptors);

  //MarkKeyPoints(img, keypoints);
  //ImgWrite("../result.bmp", img);

  delete orbd;
  img.Release();
#endif
}