#include <fstream>
#include "nano.h"

int main(void) {
  std::ifstream ifs("test.obj");
  Nano::ObjParser op(ifs);
  return 0;
}
