#include "err.h"
#include <iostream>

using namespace std;

void Err(std::string err_m) {
  cerr << "\n" + err_m + "\n" <<
          "Press Enter to exit the program.\n";
  while (cin.get() != '\n') {}
  exit(1);
}