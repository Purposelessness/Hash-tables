#include <cmath>
#include <fstream>
#include <sstream>

#include "ChainMethod.h"
#include "DoubleHashing.h"
#include "Test.h"

struct Tests {
  Tests(size_t table_size, TestOutput dh_test, TestOutput cm_test)
      : table_size(table_size), double_hashing_test(dh_test),
        chain_method_test(cm_test) {}

  size_t table_size;
  TestOutput double_hashing_test;
  TestOutput chain_method_test;
};

void testFile(const std::string& filename, const std::string& out_filename) {
  std::string out_insert_filename = out_filename + "_insert.txt";
  std::string out_erase_filename = out_filename + "_erase.txt";
  std::string out_contains_filename = out_filename + "_contains.txt";
  std::ofstream out_insert_file(out_insert_filename);
  std::ofstream out_erase_file(out_erase_filename);
  std::ofstream out_contains_file(out_contains_filename);
  if (!out_insert_file.good() || !out_erase_file.good() ||
      !out_contains_file.good()) {
    std::cout << "File error: " << out_filename << '\n';
    return;
  }

  std::cout << std::string(10, '-') << '\n';
  std::cout << filename << '\n';
  std::cout << std::string(10, '-') << '\n';
  std::ifstream file(filename);
  if (!file.good()) {
    std::cout << "File error: " << filename << '\n';
    return;
  }
  std::vector<std::string> dict;
  for (std::string line; std::getline(file, line);) {
    dict.emplace_back(line);
  }

  static constexpr int kMaxPower = 23;

  std::vector<Tests> tests;
  for (int i = 4; i < kMaxPower; i += 2) {
    auto size = static_cast<size_t>(std::pow(2, i));
    auto dh_test =
        testHashTable<DoubleHashing, std::string>("Double hashing", dict, size);
    auto cm_test =
        testHashTable<ChainMethod, std::string>("Chain method", dict, size);
    tests.emplace_back(size, dh_test, cm_test);
  }

  for (const auto& test : tests) {
    std::cout << test.table_size << ":\n"
              << static_cast<std::string>(test.double_hashing_test) << '\n'
              << static_cast<std::string>(test.chain_method_test) << "\n\n";
    std::stringstream ss;
    ss << test.table_size << '\t'
       << test.double_hashing_test.insertion_test.time << '\t'
       << test.chain_method_test.insertion_test.time << '\t'
       << test.double_hashing_test.insertion_test.avg_iterations << '\t'
       << test.chain_method_test.insertion_test.avg_iterations << '\t'
       << test.double_hashing_test.insertion_test.max_iterations << '\t'
       << test.chain_method_test.insertion_test.max_iterations << '\n';
    out_insert_file << ss.str();
    ss.str(std::string{});
    ss << test.table_size << '\t'
       << test.double_hashing_test.erasing_test.time << '\t'
       << test.chain_method_test.erasing_test.time << '\t'
       << test.double_hashing_test.erasing_test.avg_iterations << '\t'
       << test.chain_method_test.erasing_test.avg_iterations << '\t'
       << test.double_hashing_test.erasing_test.max_iterations << '\t'
       << test.chain_method_test.erasing_test.max_iterations << '\n';
    out_erase_file << ss.str();
    ss.str(std::string{});
    ss << test.table_size << '\t'
       << test.double_hashing_test.contains_test.time << '\t'
       << test.chain_method_test.contains_test.time << '\t'
       << test.double_hashing_test.contains_test.avg_iterations << '\t'
       << test.chain_method_test.contains_test.avg_iterations << '\t'
       << test.double_hashing_test.contains_test.max_iterations << '\t'
       << test.chain_method_test.contains_test.max_iterations << '\n';
    out_contains_file << ss.str();
    ss.str(std::string{});
  }
}

int main() {
  testFile("easy_dict.txt", "easy_results");
  testFile("hard_dict.txt", "hard_results");
  return 0;
}
