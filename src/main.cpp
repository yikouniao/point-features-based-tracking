#include "orb.h"
#include <iostream>
#include "test-cv.h"

#define TEST 1

int main(int argc, char** argv) {
#if TEST
  test();
#else
  ORB();
#endif
}