#include "orb.h"
#include <iostream>
#include "test-cv.h"
#include "err.h"
#include "file.h"
#include "types.h"

using namespace std;

#define TEST 0

string f_name = "../simu0075.bmp";
//string f_name = "../Homura.bmp";

int main(int argc, char** argv) {
#if TEST
  test(f_name);
#else
  Mat img = ImgRead(f_name);
  ORBDescriptor* orbd = new ORBDescriptor();
  vector<KeyPoint> keypoints;
  orbd->Detect(img, keypoints);
  //MarkKeyPoints(img, keypoints);
  //ImgWrite("../Homura1.bmp", img);
  delete orbd;
  img.Release();
#endif
}