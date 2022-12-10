#ifndef HASH_TABLES__HASH_H_
#define HASH_TABLES__HASH_H_

#include <string>

size_t hashHorner(const std::string& s, size_t k_table_size, size_t k_key);

struct H1 {
  size_t operator()(const std::string& s, size_t table_size);
};

struct H2 {
  size_t operator()(const std::string& s, size_t table_size);
};

#endif  // HASH_TABLES__HASH_H_