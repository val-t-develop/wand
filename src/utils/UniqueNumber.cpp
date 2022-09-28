#include "UniqueNumber.hpp"

static int nextUniqueNumber = 0;

int getNextUniqueNumber() {
  int tmp = nextUniqueNumber;
  nextUniqueNumber++;
  return tmp;
}