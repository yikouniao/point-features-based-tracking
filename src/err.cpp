#include "err.h"
#include <iostream>

void Err(std::string err_m) {
  std::cerr << "\n" + err_m + "\n" <<
               "Press Enter to exit the program.\n";
  while (std::cin.get() != '\n') {}
  exit(1);
}