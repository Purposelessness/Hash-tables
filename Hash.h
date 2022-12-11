#ifndef HASH_TABLES__HASH_H_
#define HASH_TABLES__HASH_H_

#include <string>

size_t hashFunction1(std::string_view s, const size_t kTableSize,
                     const size_t kKey);

template <typename T, typename F>
concept THash = requires(T t) {
  {
  t(std::declval<F>(), std::declval<size_t>())
  } -> std::same_as<size_t>;
};

struct H1 {
  size_t operator()(std::string_view s, size_t table_size) const;
};

struct H2 {
  size_t operator()(std::string_view s, size_t table_size) const;
};

#endif  // HASH_TABLES__HASH_H_