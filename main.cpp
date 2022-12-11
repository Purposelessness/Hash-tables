#include <iostream>
#include <fstream>
#include <ctime>

#include "ChainMethod.h"
#include "DoubleHashing.h"

int main() {
  std::ifstream file("dictionary.txt");
  std::vector<std::string> dict;
  for (std::string line; std::getline(file, line);) {
    dict.emplace_back(line);
  }

  DoubleHashing<std::string> double_hashing;
  clock_t time = clock();
  for (const auto& s : dict) {
    double_hashing.insert(s);
  }
  time = clock() - time;
  std::cout << "Double hashing: " << time << '\n';
  double_hashing.printSize();

  ChainMethod<std::string> chain_method;
  time = clock();
  for (const auto& s : dict) {
    chain_method.insert(s);
  }
  time = clock() - time;
  std::cout << "Chain method: " << time << '\n';
  chain_method.printSize();

  return 0;
}
