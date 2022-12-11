#ifndef HASH_TABLES__HASH_H_
#define HASH_TABLES__HASH_H_

#include <string>

size_t hashHorner(const std::string& s, size_t k_table_size, size_t k_key);

template <typename T, typename F>
concept THash = requires(T t) {
  {
  t(std::declval<F>(), std::declval<size_t>())
  } -> std::same_as<size_t>;
};

struct H {
  size_t operator()(const std::string& s, size_t table_size) const;
};

struct H1 {
  size_t operator()(const std::string& s, size_t table_size) const;
};

struct H2 {
  size_t operator()(const std::string& s, size_t table_size) const;
};

#endif  // HASH_TABLES__HASH_H_