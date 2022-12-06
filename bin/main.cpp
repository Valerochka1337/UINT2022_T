
#include <lib/number.h>
#include <iostream>

int main() {
  uint_least8_t _t;
  uint2022_t value1, value2;
  value1 = from_string("22349999999990");
  value2 = from_string("22349999999990");
  if (value1 != value2) std::cout << "YES";
  else std::cout << "NO";

  return 0;
}
