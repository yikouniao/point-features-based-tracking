#include "orb.h"
#include <iostream>
#include "test-cv.h"
#include "err.h"
#include "file.h"

#define TEST 0

int main(int argc, char** argv) {
#if TEST
  test();
#else
  ImgRead("");
  //ORB();
#endif
}