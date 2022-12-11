#include <ctime>
#include <fstream>
#include <iostream>

#include "ChainMethod.h"
#include "DoubleHashing.h"

template <template <typename> class T, typename F>
concept THashTable = requires(T<F> t) {
                       t.insert(std::declval<F>());
                       t.erase(std::declval<F>());
                       { t.contains(std::declval<F>()) } -> std::same_as<bool>;
                       t.printSize();
                     };

template <template <typename> class T, typename F>
  requires THashTable<T, F>
void testHashTable(const std::vector<std::string>& test_data,
                   const char* name) {
  T<F> hash_table;
  std::cout << "------------------\n";
  std::cout << name << '\n';
  std::cout << "------------------\n";
  clock_t time = clock();
  for (const auto& s : test_data) {
    hash_table.insert(s);
  }
  time = clock() - time;
  std::cout << "Inserting: " << time << '\n';
  hash_table.printSize();
  time = clock();
  for (const auto& s : test_data) {
    hash_table.erase(s);
  }
  time = clock() - time;
  std::cout << "Erasing: " << time << '\n';
  hash_table.printSize();
  std::cout << "------------------\n\n";
}

int main() {
  std::ifstream file("dictionary.txt");
  std::vector<std::string> dict;
  for (std::string line; std::getline(file, line);) {
    dict.emplace_back(line);
  }

  testHashTable<DoubleHashing, std::string>(dict, "Double hashing");
  testHashTable<ChainMethod, std::string>(dict, "Chain method");

  return 0;
}
