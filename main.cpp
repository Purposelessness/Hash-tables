#include <iostream>

#include "DoubleHashing.h"

int main() {
  DoubleHashing<std::string> set;
  set.print();
  set.insert("aboba");
  set.insert("aboba");
  set.print();
  std::cout << set.contains("aboba");
  return 0;
}
