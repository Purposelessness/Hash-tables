#include "Hash.h"

size_t hashHorner(const std::string& s, const size_t kTableSize,
                  const size_t kKey) {
  size_t hash = 0;
  for (const auto& c : s) {
    hash = (kKey * hash + c) % kTableSize;
  }
  hash = (hash * 2 + 1) % kTableSize;
  return hash;
}

size_t H1::operator()(const std::string& s, const size_t kTableSize) {
  return hashHorner(s, kTableSize, kTableSize + 1);
}

size_t H2::operator()(const std::string& s, const size_t kTableSize) {
  return hashHorner(s, kTableSize, kTableSize - 1);
}
