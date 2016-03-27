#include "orb.h"
#include <iostream>
#include "test-cv.h"
#include "err.h"
#include "file.h"

#define TEST 1

int main(int argc, char** argv) {
#if TEST
  //test();
#else
  Mat a = ImgRead("Witch_Yoshino.bmp");
  //ImgWrite("Witch_Yoshino.bmp", a);
  a.Release();
  //ORB();
#endif
}