#include "orb.h"
#include <iostream>
#include "test-cv.h"
#include "err.h"
#include "file.h"
#include "types.h"

using namespace std;

#define TEST 1

int main(int argc, char** argv) {
#if TEST
  test();
#else
  Mat img = ImgRead("../Homura.bmp");
  ORBDescriptor* orbd = new ORBDescriptor();
  vector<KeyPoint> keypoints;
  orbd->Detect(img, keypoints);
  MarkKeyPoints(img, keypoints);
  delete orbd;
  img.Release();
#endif
}