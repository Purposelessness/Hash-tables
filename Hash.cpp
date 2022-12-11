#include "Hash.h"

constexpr int kMagicNumber1 = 5381;
constexpr int kMagicNumber2 = 5383;

size_t hashFunction1(std::string_view s, const size_t kTableSize,
                  const size_t kKey) {
  size_t hash = 0;
  for (const auto& c : s) {
    hash = (kKey * hash + c) % kTableSize;
  }
  hash = (hash * 2 + 1) % kTableSize;
  return hash;
}

size_t H1::operator()(std::string_view s, const size_t kTableSize) const {
  return hashFunction1(s, kTableSize, kMagicNumber1);
}

size_t H2::operator()(std::string_view s, const size_t kTableSize) const {
  return hashFunction1(s, kTableSize, kMagicNumber2);
}
